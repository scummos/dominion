#pragma once

#include "logger.h"

#include <QMainWindow>
#include <QFutureWatcher>
#include <QTimer>
#include <QElapsedTimer>

#include <memory>

namespace KSyntaxHighlighting {
    class Repository;
}

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    MainWindow();
    ~MainWindow();

    bool eventFilter(QObject* watched, QEvent* event) override;

    void recompute();
    void recomputeRefined(int count);
    void showAvailableResults();
    void showGraphDelayed();
    void showGraph();
    PerTurnLogData graphDataType() const;

    void loadFile();
    void saveFile();
    void saveFileAs();
    void loadEnemy();
    QString readFile(QString const& fn);

    void closeEvent(QCloseEvent* event) override;

private:
    void loadEnemy(const QString& filename);

private Q_SLOTS:
    void appendLog(QString const& html);
    void displayLog();

private:
    class Ui_MainWindow* ui;
    std::shared_ptr<KSyntaxHighlighting::Repository> m_repo;

    QFutureWatcher<int> m_watcher;
    QFuture<int> m_winners;

    struct {
        int min, max;
        int value;
    } m_progress;

    QVector<double> m_xdata[5], m_ydata[5];
    QString m_filename = ":/strategy/rebuild.buylist";
    QString m_enemyFilename = ":/strategy/smithy_bm.buylist";

    QTimer m_graphTimer;
    QElapsedTimer m_perfTimer;

    QString m_log;
    QTimer m_logTimer;
};
