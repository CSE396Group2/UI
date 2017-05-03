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
#include "connectionthread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ConnectionThread *connectionTh;
    ~MainWindow();

private:

    QTimer *timer;
    int countTime=0;
    QTcpSocket *socket = NULL;
    QTcpServer *server;
    char onClickedMessage[64];
    bool isStartButtonClicked = false;
    bool isStopButtonClicked = false;

    Scene2d *scene2d;
    Drawer *drawer;
    int portNumber;

    void startTimer();
    void stopTimer();
public slots:
    void isConnect();

private slots:
    void on_startButton_clicked();
    void startServer();
    void on_stopButton_clicked();
    void counterTimer();

    void on_portButton_clicked();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
