#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTimer>

#include <QtCore>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTcpSocket>
#include <QTcpServer>

#include "scene2d.h"
#include "connectionthread.h"
#include "coordinatebrowserth.h"
#include "scene2dth.h"
#include "icsrulerwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ConnectionThread *connectionTh;
    CoordinateBrowserTh *coorBrowTh;
    Scene2dTh *scene2dTh;
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QTcpServer *server;
    QTcpSocket *socket = NULL;
    QString ipNumber;
    Scene2d *scene2d; //bunu kaldır
    QTimer timer2d;
    QTimer dataTimerFirst;
    QTimer dataTimerSecond;
    QMutex mutex;
    QString onClickedMessage;
    ICSRulerWidget ruller;
    int countTime=0;
    int portNumber;
    int comPortNumber = -1;
    int routeX = 0;
    int routeY = 0;
    int rotation = 0;
    bool isFound = false;
    bool autoMode;
    bool isStartButtonClicked = false;
    bool isStopButtonClicked = false;
    bool isResetButtonClicked = false;


public slots:
    void rotatePic(int angle);

private slots:
    void on_startButton_clicked();
    void startServer();
    void on_stopButton_clicked();
    void counterTimer();
    void startTimer();
    void stopTimer();
    void restartTimer();
    void isConnect();
    void sendData();
    void update2DCoordinates();
    void updateBrow();
    void realtimeDataSlotFirst();
    void realtimeDataSlotSecond();
    void on_portButton_clicked();
    void on_resetButton_clicked();
    void on_upButton_clicked();
    void on_rightButton_clicked();
    void on_leftButton_clicked();
    void on_downButton_clicked();
};

#endif // MAINWINDOW_H
