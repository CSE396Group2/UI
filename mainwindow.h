#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    char onClickedMessage[64];
    bool isStartButtonClicked = false;
    bool isStopButtonClicked = false;
private slots:
    void on_startButton_clicked();
    void startServer();
    void on_stopButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
