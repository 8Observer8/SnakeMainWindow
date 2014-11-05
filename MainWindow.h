#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void signalNewGame();

private slots:
    void slotShowStatus( const QString &status );

    void on_actionNew_triggered();

    void on_actionExit_triggered();

private:
    Ui::MainWindow *ui;
    QLabel *m_statusLabel;
};

#endif // MAINWINDOW_H
