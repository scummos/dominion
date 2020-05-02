#pragma once

#include "logger.h"

#include <QMainWindow>
#include <QFutureWatcher>

#include <memory>

namespace KSyntaxHighlighting {
    class Repository;
}

class MainWindow : public QMainWindow {
public:
    MainWindow();
    ~MainWindow();

    bool eventFilter(QObject* watched, QEvent* event) override;

    void recompute();
    void showAvailableResults();
    void showGraph();
    PerTurnLogData graphDataType() const;

    void loadFile();
    void saveFile();
    void saveFileAs();
    void selectDefaultEnemy(int index);
    void loadEnemy();
    QString readFile(QString const& fn);

private:
    void loadEnemy(const QString& filename);

private:
    class Ui_MainWindow* ui;
    std::shared_ptr<KSyntaxHighlighting::Repository> m_repo;

    QFutureWatcher<int> m_watcher;
    QFuture<int> m_winners;

    struct {
        int min, max;
        int value;
    } m_progress;

    QVector<double> m_xdata, m_ydata;
    QString m_filename = ":/strategy/rebuild.buylist";
    QString m_enemyFilename = ":/strategy/smithy_bm.buylist";
};
