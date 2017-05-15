#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene2d = new Scene2d(this);
    coorBrowTh = new CoordinateBrowserTh(this);
    scene2dTh = new Scene2dTh(this);

    ui->graphicsView->setScene(scene2d);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->auModeCheckBox->setCheckable(true);

    //x-t, y-t graphics
    ui->customPlot->addGraph(); // blue line
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));
    ui->customPlotSecond->addGraph(); // blue line
    ui->customPlotSecond->addGraph(); // red line
    ui->customPlotSecond->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->customPlotSecond->graph(1)->setPen(QPen(QColor(255, 110, 40)));

    QSharedPointer <QCPAxisTickerTime> timeTickerFirst(new QCPAxisTickerTime);
    QSharedPointer <QCPAxisTickerTime> timeTickerSecond(new QCPAxisTickerTime);
    timeTickerFirst->setTimeFormat("%h:%m:%s");
    timeTickerSecond->setTimeFormat("%h:%m:%s");
    ui->customPlot->xAxis->setTicker(timeTickerFirst);
    ui->customPlotSecond->xAxis->setTicker(timeTickerSecond);
    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlotSecond->axisRect()->setupFullAxesBox();
    ui->customPlot->yAxis->setRange(0, 1023);
    ui->customPlotSecond->yAxis->setRange(0, 800);

    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlotSecond->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlotSecond->xAxis2,
            SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlotSecond->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlotSecond->yAxis2,
            SLOT(setRange(QCPRange)));

    connect(&dataTimerFirst, SIGNAL(timeout()), this, SLOT(realtimeDataSlotFirst()));
    connect(&dataTimerSecond, SIGNAL(timeout()), this, SLOT(realtimeDataSlotSecond()));
    // end x-t y-t graphics

    dataTimerFirst.start();
    dataTimerSecond.start();

    connectionTh = new ConnectionThread(this);
    socket = new QTcpSocket(this);
    connect(connectionTh,SIGNAL(startConnection()),this,SLOT(isConnect()),Qt::DirectConnection);
    connect(coorBrowTh,SIGNAL(updateBrowser()),this,SLOT(updateBrow()));
    connect(scene2dTh,SIGNAL(update2DScene()),this,SLOT(update2DCoordinates()));
    connect(&timer2d,SIGNAL(timeout()),this,SLOT(update2DSscene()));

    server = new QTcpServer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startTimer()
{
    // create a timer
    timer = new QTimer(this);
    // setup signal and slot
    connect(timer, SIGNAL(timeout()),this, SLOT(counterTimer()));
    //msec
    timer->start(10);

}

void MainWindow::counterTimer()
{
    countTime++;
    ui->timerLabel->setText(QString::number((countTime/100)/60)+":"+QString::number((countTime/100)%60)+":"+QString::number(countTime%100));

}
void MainWindow::stopTimer()
{
    timer->stop();
}

void MainWindow::restartTimer()
{
    timer->stop();
    countTime=0;
    ui->timerLabel->setText(QString::number(0)+":"+QString::number(0)+":"+QString::number(0));

}

void MainWindow::startServer(){
    qDebug() << "Server started" ;
}


void MainWindow::isConnect()
{
    //QHostAddress hostAddr(ipNumber);
    QHostAddress hostAddr("192.168.43.187");
    QByteArray readData;
    char* stringData;
    char param[3];
    socket->connectToHost(hostAddr,portNumber);

    if(socket->waitForConnected(5000)){
        timer2d.start();
        timer2d.setInterval(17);
        socket->write(onClickedMessage.toLatin1());
        socket->waitForBytesWritten(3000);

        onClickedMessage = QString::number(comPortNumber);

        socket->write(onClickedMessage.toLatin1());
        socket->waitForBytesWritten(100);

        while(!isStopButtonClicked){
            socket->waitForReadyRead(3000);
            socket->bytesAvailable();
            readData = socket->readAll();
            stringData = readData.data();
            mutex.lock();
            qDebug() <<"stringData: "<< stringData << endl;

            strcpy(param,strtok(stringData," ,"));
            routeX = atoi(param);

            strcpy(param,strtok(NULL," ,"));
            routeY = atoi(param);

            strcpy(param,strtok(NULL," ,"));

            if(strcmp(param,"t") == 0){
                isFound = true;
            }else if(strcmp(param,"f") == 0){
                isFound = false;
            }else {
                qDebug() << "Invalid value" << endl;
            }

            strcpy(param,strtok(NULL," ,"));
            rotation = atoi(param);

            qDebug() << "routeX:-" << routeX << "routeY: " << routeY << "isFound: " << isFound << "rotaion: " << rotation <<"-" << endl;
            scene2d->setBoard(routeX,routeY);
            scene2dTh->start();
            coorBrowTh->start();
            mutex.unlock();
            if(isStopButtonClicked){
                qDebug() << "stop button skaldjlds";
                socket->close();
                break;
            }
        }
    }else{
        qDebug()<< "Connection lost";
    }
}

void MainWindow::sendData(){
    socket->write(onClickedMessage.toLatin1());
    socket->waitForBytesWritten(3000);
}

void MainWindow::updateBrow()
{
    ui->coordinatBrowser->append("X:"+QString::number(routeX)+"\tY:"+QString::number(routeY));
}

void MainWindow::realtimeDataSlotFirst() {
    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed() / 1000.0; // time elapsed since start of demo, in seconds

    static double lastPointKey = 0;
    if (key - lastPointKey > 0.5) // at most add point every 2 ms
    {
        // add data to lines:
        ui->customPlot->graph(0)->addData(key, routeX);
        lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->customPlot->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->customPlot->replot();

    // calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key - lastFpsKey > 2) // average fps over 2 seconds
    {
        ui->statusBar->showMessage(
                QString("%1 FPS, Total Data points: %2")
                        .arg(frameCount / (key - lastFpsKey), 0, 'f', 0)
                        .arg(ui->customPlot->graph(0)->data()->size()/*ui->customPlot->graph(1)->data()->size()*/), 0);
        lastFpsKey = key;
        frameCount = 0;
    }
}

void MainWindow::realtimeDataSlotSecond() {
    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed() / 1000.0; // time elapsed since start of demo, in seconds

    static double lastPointKey = 0;
    if (key - lastPointKey > 0.5) // at most add point every 2 ms
    {
        // add data to lines:
        ui->customPlotSecond->graph(0)->addData(key, routeY);
        lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->customPlotSecond->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->customPlotSecond->replot();

    // calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key - lastFpsKey > 2) // average fps over 2 seconds
    {
        ui->statusBar->showMessage(
                QString("%1 FPS, Total Data points: %2")
                        .arg(frameCount / (key - lastFpsKey), 0, 'f', 0)
                        .arg(ui->customPlotSecond->graph(
                                0)->data()->size()/*ui->customPlot->graph(1)->data()->size()*/), 0);
        lastFpsKey = key;
        frameCount = 0;
    }
}


void MainWindow::on_startButton_clicked()
{
    if(isStartButtonClicked == false){
        isStartButtonClicked = true;
        isStopButtonClicked = false;
        onClickedMessage = "x";
        qDebug() << "startButton was clicked";
        startTimer();//start timer
        if(ui->auModeCheckBox->isChecked()){
            qDebug() << "true";
            ui->auModeCheckBox->setChecked(true);
            autoMode = true;
            ui->upButton->setDisabled(true);
            ui->downButton->setDisabled(true);
            ui->rightButton->setDisabled(true);
            ui->leftButton->setDisabled(true);
        }else {
            qDebug() << "false";
            ui->auModeCheckBox->setChecked(false);
            autoMode = false;
            ui->upButton->setDisabled(false);
            ui->downButton->setDisabled(false);
            ui->rightButton->setDisabled(false);
            ui->leftButton->setDisabled(false);
        }
        connectionTh->start();
        qDebug() << "signal";
    }
}

void MainWindow::on_stopButton_clicked()
{
    if(isStopButtonClicked == false){
        isStartButtonClicked = false;

        stopTimer();//stop timer

        isStopButtonClicked = true;
        onClickedMessage = "q";
        sendData();
        qDebug() << "stopButton was clicked";
    }
}

void MainWindow::on_portButton_clicked()
{
    QString temp = ui->portNumberLine->text();

    portNumber = temp.toInt();
    qDebug("portNumber: ");
    qDebug() << temp.toLatin1();
    ipNumber = ui->ipNumberLine->text();
    qDebug("ipNumber: ");
    qDebug(ipNumber.toLatin1());

    QString comPort = ui->comPortBox->currentText();

    qDebug(comPort.toLatin1());

    if(comPort.compare("COM1") == 0){
        comPortNumber = 0;
    } else if (comPort.compare("COM2") == 0){
        comPortNumber = 1;
    } else if (comPort.compare("COM3") == 0){
        comPortNumber = 2;
    } else if (comPort.compare("COM4") == 0){
        comPortNumber = 3;
    } else if (comPort.compare("COM5") == 0){
        comPortNumber = 4;
    } else if (comPort.compare("COM6") == 0){
        comPortNumber = 5;
    } else if (comPort.compare("COM7") == 0){
        comPortNumber = 6;
    } else if (comPort.compare("COM8") == 0){
        comPortNumber = 7;
    } else if (comPort.compare("COM9") == 0){
        comPortNumber = 8;
    } else if (comPort.compare("ttyACM0") == 0){
        comPortNumber = 24;
    } else if (comPort.compare("ttyACM1") == 0){
        comPortNumber = 25;
    }
    qDebug("comportNumber: ");
    qDebug() << comPortNumber;
}

void MainWindow::on_resetButton_clicked()
{
    if(isResetButtonClicked == false){
        restartTimer();//start timer
        isResetButtonClicked = true;
        isStartButtonClicked = false;
        isStopButtonClicked = false;
     }
}

void MainWindow::on_upButton_clicked()
{
    onClickedMessage = "w";
    sendData();
}

void MainWindow::on_rightButton_clicked()
{
    onClickedMessage = "d";
    sendData();
}

void MainWindow::on_leftButton_clicked()
{
    onClickedMessage = "a";
    sendData();
}

void MainWindow::on_downButton_clicked()
{
    onClickedMessage = "s";
    sendData();
}

void MainWindow::update2DCoordinates(){
    scene2d->draw();
}
