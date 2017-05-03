#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTimer>

#include <QtCore>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "scene2d.h"
#include "drawer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

    QTimer *timer;
    int countTime=0;
    char onClickedMessage[64];
    bool isStartButtonClicked = false;
    bool isStopButtonClicked = false;

    Scene2d *scene2d;
    Drawer *drawer;
    int portNumber;
private slots:

    void startTimer();
    void counterTimer();
    void stopTimer();

    void on_startButton_clicked();
    void startServer();
    void on_stopButton_clicked();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
