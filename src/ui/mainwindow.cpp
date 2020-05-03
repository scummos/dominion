#include "mainwindow.h"

#include "ui_mainwindow.h"

#include "parser.h"
#include "game.h"

#include <syntaxhighlighter.h>
#include <repository.h>
#include <definition.h>
#include <theme.h>

#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>

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
    connect(ui->enemyFirst, &QCheckBox::toggled, this, &MainWindow::recompute);
    connect(&m_watcher, &decltype(m_watcher)::progressRangeChanged, this, [this](int min, int max) {
        m_progress.min = min;
        m_progress.max = max;
    });
    connect(&m_watcher, &decltype(m_watcher)::progressValueChanged, this, [this](int value) {
        m_progress.value = value;
        ui->statusbar->showMessage(QString("Running: %1/%2%3")
            .arg(m_progress.value)
            .arg(m_progress.max)
            .arg(m_progress.max == m_progress.value ? ", done." : ""));
        showAvailableResults();
    });

    m_graphTimer.setInterval(100);
    m_graphTimer.setSingleShot(true);
    connect(&m_graphTimer, &QTimer::timeout, this, &MainWindow::showGraph);

    connect(ui->graphCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::showGraph);

    connect(ui->actionLoad, &QAction::triggered, this, &MainWindow::loadFile);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveFile);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::saveFileAs);
    connect(ui->selectEnemy, &QPushButton::clicked, this, QOverload<>::of(&MainWindow::loadEnemy));

    connect(ui->buttonRefine, &QPushButton::clicked, this, [this]() {
        recomputeRefined(100000);
    });

    connect(ui->jumpToError, &QPushButton::clicked, this, [this]() {
        // TODO this doesn't work, dunno why
        auto block = ui->strategyEdit->document()->findBlockByLineNumber(ui->jumpToError->property("goto").toInt());
        auto cursor = QTextCursor(block);
        ui->strategyEdit->setTextCursor(cursor);
    });
    ui->jumpToError->setVisible(false);

    auto enemyPresetMenu = new QMenu(this);
    ui->selectEnemy->setMenu(enemyPresetMenu);
    for (auto preset: {":strategy/bm.buylist", ":strategy/smithy_bm.buylist"}) {
        enemyPresetMenu->addAction(preset, [this, preset]() {
            loadEnemy(preset);
        });
    }

    auto grid = new QwtPlotGrid();
    grid->setMajorPen("#CCCCCC");
    grid->setMinorPen("#DDDDDD", 1.0, Qt::DashLine);
    grid->enableXMin(true);
    grid->enableYMin(true);
    grid->attach(ui->qwtPlot);

    loadEnemy(m_enemyFilename);
    recompute();
}

void MainWindow::showGraphDelayed()
{
    if (!m_graphTimer.isActive()) {
        m_graphTimer.start();
    }
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
    m_filename = fn;
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
    ui->statusbar->showMessage(QString("Saved to %1.").arg(m_filename));
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
    ui->selectedEnemy->setText(filename.section("/", -1, -1));
    recompute();
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

    showGraphDelayed();
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
    QPen pens[] = {QPen(Qt::blue), QPen(Qt::lightGray)};

    for (int player = 0; player < 2; player++) {
        QwtPlotCurve* curve = new QwtPlotCurve();
        auto graph = Logger::instance()->computeTurnGraph(player, graphDataType());

        int turn = 0;
        // setSamples takes a constref, this is still a bit of a wtf for me ...
        m_xdata[player].clear();
        m_ydata[player].clear();
        for (auto entry: graph) {
            m_xdata[player].append(turn++);
            m_ydata[player].append(entry.value);
        }
        curve->setSamples(m_xdata[player], m_ydata[player]);
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
    recomputeRefined(10000);
}

void MainWindow::recomputeRefined(int games)
{
    if (m_watcher.isRunning()) {
        m_watcher.cancel();
        m_watcher.waitForFinished();
    }

    std::pair<BuylistCollection, StrategyCollection> args1;
    ui->errorStack->setCurrentIndex(0);
    ui->syntaxError->setText({});
    try {
        args1 = parseBuylist(ui->strategyEdit->toPlainText().toUtf8().data());
    }
    catch (BuylistParseError e) {
        ui->errorStack->setCurrentIndex(1);
        ui->syntaxError->setText(QString("Parse error in line %1.%2: %3")
            .arg(e.beginLine)
            .arg(e.beingColumn)
            .arg(QString::fromStdString(e.error))
        );
        ui->jumpToError->setProperty("goto", e.beginLine);
        return;
    }
    auto args2 = parseBuylist(readFile(m_enemyFilename).toUtf8().data());

    QVector<int> gameNumber(games);
    std::iota(gameNumber.begin(), gameNumber.end(), 0);
    auto logger = Logger::instance(games);
    logger->clear();

    auto enemyFirst = ui->enemyFirst->isChecked();
    std::function<int(int)> func = [enemyFirst, args1, args2, logger](int) {
        Game game({"buylist", "buylist"}, {std::any(args1), std::any(args2)});
        game.setFirstPlayer(enemyFirst ? 1 : 0);
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


