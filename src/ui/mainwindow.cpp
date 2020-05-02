#include "mainwindow.h"

#include "ui_mainwindow.h"

#include "parser.h"
#include "game.h"

#include <syntaxhighlighter.h>
#include <repository.h>
#include <definition.h>
#include <theme.h>

#include <qwt_plot_curve.h>

#include <QDebug>
#include <QKeyEvent>
#include <QFileDialog>
#include <QTemporaryFile>
#include <QSettings>
#include <QtConcurrentMap>

auto const KEY_DIR = "buylistDir";
auto const FILENAME_FILTER = "Dominion Strategy Files (*.buylist)";

MainWindow::MainWindow()
    : ui(new Ui_MainWindow)
{
    ui->setupUi(this);

    m_repo = std::make_shared<KSyntaxHighlighting::Repository>();
    m_repo->addCustomSearchPath(":/syntax/");

    auto* highlighter = new KSyntaxHighlighting::SyntaxHighlighter(this);
    highlighter->setDefinition(m_repo->definitionForName("Buylist"));
    highlighter->setDocument(ui->strategyEdit->document());
    highlighter->setTheme(m_repo->defaultTheme());

    ui->strategyEdit->installEventFilter(this);

    ui->strategyEdit->setTabStopDistance(20); // pixels

    connect(ui->strategyEdit, &QTextEdit::textChanged, this, &MainWindow::recompute, Qt::QueuedConnection);
    connect(&m_watcher, &decltype(m_watcher)::progressRangeChanged, this, [this](int min, int max) {
        m_progress.min = min;
        m_progress.max = max;
    });
    connect(&m_watcher, &decltype(m_watcher)::progressValueChanged, this, [this](int value) {
        m_progress.value = value;
        ui->statusbar->showMessage(QString("done: %1/%2").arg(m_progress.value).arg(m_progress.max));
        showAvailableResults();
    });

    connect(ui->graphCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::showGraph);

    connect(ui->actionLoad, &QAction::triggered, this, &MainWindow::loadFile);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveFile);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::saveFileAs);
    connect(ui->enemyCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::selectDefaultEnemy);
    connect(ui->selectEnemy, &QPushButton::clicked, this, QOverload<>::of(&MainWindow::loadEnemy));

    recompute();
}

QString MainWindow::readFile(const QString& fn)
{
    QFile f(fn);
    if (!f.open(QIODevice::ReadOnly)) {
        qWarning() << "failed to open" << fn;
        return {};
    }
    return f.readAll();
}

void MainWindow::loadFile()
{
    QSettings s;
    auto dir = s.value(KEY_DIR).toString();

    QFileDialog d;
    auto fn = d.getOpenFileName(this, "Load file", dir, FILENAME_FILTER);
    if (fn.isEmpty()) {
        return;
    }
    s.setValue(KEY_DIR, fn);

    ui->strategyEdit->setText(readFile(fn));
}

void MainWindow::saveFile()
{
    if (m_filename.isEmpty()) {
        saveFileAs();
        return;
    }

    QFile f(m_filename);
    if (!f.open(QIODevice::WriteOnly)) {
        qWarning() << "failed to open" << m_filename;
        return;
    }

    f.write(ui->strategyEdit->toPlainText().toUtf8());
}

void MainWindow::saveFileAs()
{
    QSettings s;
    auto dir = s.value(KEY_DIR).toString();

    QFileDialog d;
    auto fn = d.getSaveFileName(this, "Save file as", dir, FILENAME_FILTER);
    if (fn.isEmpty()) {
        return;
    }

    s.setValue(KEY_DIR, fn);

    m_filename = fn;
    saveFile();
}

void MainWindow::loadEnemy()
{
    QSettings s;
    auto dir = s.value(KEY_DIR).toString();

    QFileDialog d;
    auto fn = d.getOpenFileName(this, "Load file", dir, FILENAME_FILTER);
    if (fn.isEmpty()) {
        return;
    }

    loadEnemy(fn);
}

void MainWindow::loadEnemy(const QString& filename)
{
    m_enemyFilename = filename;
    recompute();
}

void MainWindow::selectDefaultEnemy(int index)
{
    QString fn;
    switch (index) {
        case 0: fn = ":/strategy/smithy_bm.buylist"; break;
        case 1: fn = ":/strategy/bm.buylist"; break;
        default: return;
    }
    loadEnemy(fn);
}

void MainWindow::showAvailableResults()
{
    auto available = m_winners.resultCount();
    int winner1 = 0;
    int winner2 = 0;
    int draws = 0;
    for (int i = 0; i < available; i++) {
        auto winner = m_winners.resultAt(i);
        switch (winner) {
            case 0: winner1++; break;
            case 1: winner2++; break;
            default: draws++; break;
        }
    }
    ui->winrate->setMinimum(0);
    ui->winrate->setMaximum(available);
    ui->winrate->setValue(winner1);
    ui->draws->setMinimum(0);
    ui->draws->setMaximum(available);
    ui->draws->setValue(draws);

    showGraph();
}

PerTurnLogData MainWindow::graphDataType() const
{
    auto index = ui->graphCombo->currentIndex();
    if (index >= static_cast<int>(PerTurnLogData::Num)) {
        qWarning() << "invalid graph data requested";
        return PerTurnLogData::TurnNumber;
    }
    return static_cast<PerTurnLogData>(index);
}

void MainWindow::showGraph()
{
    ui->qwtPlot->detachItems(QwtPlotItem::Rtti_PlotCurve);
    QPen pens[] = {QPen(Qt::darkGreen), QPen(Qt::red)};

    for (int player = 0; player < 2; player++) {
        QwtPlotCurve* curve = new QwtPlotCurve();
        auto graph = Logger::instance()->computeTurnGraph(player, graphDataType());

        int turn = 0;
        // setSamples takes a constref, this is still a bit of a wtf for me ...
        m_xdata.clear();
        m_ydata.clear();
        for (auto entry: graph) {
            m_xdata.append(turn++);
            m_ydata.append(entry.value);
        }
        curve->setSamples(m_xdata, m_ydata);
        QPen pen(pens[player]);
        pen.setWidth(1);
        curve->setPen(pen);
        curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
        curve->attach(ui->qwtPlot);
    }
    ui->qwtPlot->replot();
}

void MainWindow::recompute()
{
    if (m_watcher.isRunning()) {
        m_watcher.cancel();
        m_watcher.waitForFinished();
    }

    std::pair<BuylistCollection, StrategyCollection> args1;
    try {
        args1 = parseBuylist(ui->strategyEdit->toPlainText().toUtf8().data());
    }
    catch (BuylistParseError e) {
        ui->statusbar->showMessage(QString("Parse error in line %1.%2: %3")
            .arg(e.beginLine)
            .arg(e.beingColumn)
            .arg(QString::fromStdString(e.error))
        );
        return;
    }
    auto args2 = parseBuylist(readFile(m_enemyFilename).toUtf8().data());

    auto games = 10000;
    QVector<int> gameNumber(games);
    std::iota(gameNumber.begin(), gameNumber.end(), 0);
    auto logger = Logger::instance(games);
    logger->clear();

    std::function<int(int)> func = [args1, args2, logger](int) {
        Game game({"buylist", "buylist"}, {std::any(args1), std::any(args2)});
        auto winner = game.run();

        logger->addGame(game.logData());
        return winner;
    };
    m_winners = QtConcurrent::mapped(gameNumber, func);
    m_watcher.setFuture(m_winners);
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == ui->strategyEdit) {
        if (event->type() != QEvent::KeyPress) {
            return false;
        }
        auto keyEvent = static_cast<QKeyEvent*>(event);
        switch (keyEvent->key() & ~Qt::KeyboardModifierMask) {
            case Qt::Key_Tab: {
                ui->strategyEdit->insertPlainText("\t");
                return true;
            }
            case Qt::Key_Return: {
                auto cursor = ui->strategyEdit->textCursor();
                auto block = cursor.block();
                auto text = block.text();
                auto tabs = 0;
                for (int pos = cursor.positionInBlock(); pos >= 0; pos--) {
                    auto c = text.at(pos);
                    if (c == QLatin1Char('\t')) {
                        tabs++;
                    }
                    else if (c == QLatin1Char('\n')) {
                        break;
                    }
                    else {
                        tabs = 0;
                    }
                }
                ui->strategyEdit->insertPlainText("\n" + QString("\t").repeated(tabs));
                return true;
            }
            default:
                return false;
        }
    }
    return false;
}

MainWindow::~MainWindow()
{
    delete ui;
}


