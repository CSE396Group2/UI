#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene2d = new Scene2d(this);
    coorBrowTh = new CoordinateBrowserTh(this);
    scene2dTh = new Scene2dTh(this);
    coorBrowTh = new CoordinateBrowserTh();

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
    ui->customPlot->yAxis->setRange(0, 21);
    ui->customPlotSecond->yAxis->setRange(0, 15);

    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlotSecond->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlotSecond->xAxis2,
            SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlotSecond->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlotSecond->yAxis2,
            SLOT(setRange(QCPRange)));

    connect(&dataTimerFirst, SIGNAL(timeout()), this, SLOT(realtimeDataSlotFirst()));
    connect(&dataTimerSecond, SIGNAL(timeout()), this, SLOT(realtimeDataSlotSecond()));
    // end x-t y-t graphics



    connectionTh = new ConnectionThread(this);
    socket = new QTcpSocket(this);
    connect(connectionTh,SIGNAL(startConnection()),this,SLOT(isConnect()),Qt::DirectConnection);
    connect(coorBrowTh,SIGNAL(updateBrowser()),this,SLOT(updateBrow()));
    connect(scene2dTh,SIGNAL(update2DScene()),this,SLOT(update2DCoordinates()));

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
    QHostAddress hostAddr(ipNumber);
    QByteArray readData;
    char* stringData;
    char param[3];
    socket->connectToHost(hostAddr,portNumber);

    if(socket->waitForConnected(5000)){
        timer2d.start();
        timer2d.setInterval(17);

        onClickedMessage = QString::number(comPortNumber);

        socket->write(onClickedMessage.toLatin1()); // send comport number
        socket->waitForBytesWritten(100);
        if(!autoMode){// manual mode
            onClickedMessage = "m";

        }else {
            onClickedMessage = "a";
        }

        socket->write(onClickedMessage.toLatin1());
        socket->waitForBytesWritten(100);

        socket->waitForReadyRead(100);
        socket->bytesAvailable();
        readData = socket->readAll();
        stringData = readData.data();

        qDebug() << "stringData might be p or f: " << stringData<< endl;

        if(strcmp(stringData,"f") == 0){
            qDebug()<< "comport error" << endl;
//            if(comPortNumber == 24){
//                socket->write(25);
//                socket->waitForBytesWritten(100);
//            }else if (comPortNumber == 25){
//                socket->write(24);
//                socket->waitForBytesWritten(100);
//            }
        }else {
            onClickedMessage = 'p';
            qDebug()<< "***********************************************" <<endl;
            socket->write(onClickedMessage.toLatin1());
            socket->waitForBytesWritten(100);

            while(!isStopButtonClicked){
                socket->waitForReadyRead(17); //waiting data
                socket->bytesAvailable();
                readData = socket->readAll();
                stringData = readData.data();

                if( strlen(stringData) == 11 ){
                    //qDebug() <<"stringData: "<< stringData << endl;
                    mutex.lock();
                    try{
                        strcpy(param,strtok(stringData," ,"));
                        routeX = atoi(param);
                        strcpy(param,strtok(NULL," ,"));
                        routeY = atoi(param);

                        strcpy(param,strtok(NULL," ,"));

                        rotation = atoi(param) - 200;
                        if( rotation != 799 && rotation < 799 && !isFound){
                            qDebug() << "readData: " <<readData.data() << endl;
                            ui->picLabel->setPixmap(QPixmap(":/images/cin2.bmp"));
                            ui->picLabel->show();
                            rotatePic(rotation);
                            isFound = true;
                            foundRouteX = routeX;
                            foundRouteY = routeY;
                            foundRotaion = rotation;
                            coorBrowTh->start();
                        }
                        qDebug() << "routeX:" << routeX << "routeY: " << routeY << "rotaion: " << rotation << endl;
                    }catch(...){
                        qDebug()<< "parse error" << endl;
                    }

                    scene2d->setBoard(routeY,routeX);
                    scene2dTh->update2DScene();
                    mutex.unlock();
                    if(isStopButtonClicked){
                        qDebug() << "stop button skaldjlds";
                        socket->close();
                        break;
                    }
                }
            }
        }
    }else{
        qDebug()<< "Connection lost";
    }
}

void MainWindow::sendData(){
    socket->write(onClickedMessage.toLatin1());
    qDebug() << "sent data: " <<onClickedMessage.toLatin1()<< endl;
    socket->waitForBytesWritten(3000);
}

void MainWindow::updateBrow()
{
    ++foundedCounter;
    ui->coordinatBrowser->append(QString::number(foundedCounter) + ") X:"+QString::number(foundRouteX)+"\tY:"+QString::number(foundRouteY) + "\t°" + QString::number(foundRotaion));
}

void MainWindow::realtimeDataSlotFirst() {
    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed() / 1000.0; // time elapsed since start of demo, in seconds

    static double lastPointKey = 0;
    if (key - lastPointKey > 0.5) // at most add point every 2 ms
    {
        // add data to lines:
        ui->customPlot->graph(0)->addData(key, routeY/30);
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
                        .arg(ui->customPlot->graph(0)->data()->size()), 0);
        lastFpsKey = key;
        frameCount = 0;
    }
}

void MainWindow::rotatePic(int angle){

       QApplication::processEvents();

       QPixmap ship(":/images/cin2.bmp");
       QPixmap rotate(ship.size());

       angle =360-angle;


       QPainter p(&rotate);
       p.setRenderHint(QPainter::Antialiasing);
       p.setRenderHint(QPainter::SmoothPixmapTransform);
       p.setRenderHint(QPainter::HighQualityAntialiasing);
       p.translate(rotate.size().width() / 2, rotate.size().height() / 2);
       p.rotate(angle);
       p.translate(-rotate.size().width() / 2, -rotate.size().height() / 2);

       p.drawPixmap(0, 0, ship);
       p.end();

       ui->picLabel->setPixmap(rotate);

       //couner++;
     // rotate

}

void MainWindow::realtimeDataSlotSecond() {
    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed() / 1000.0; // time elapsed since start of demo, in seconds

    static double lastPointKey = 0;
    if (key - lastPointKey > 0.5) // at most add point every 2 ms
    {
        // add data to lines:
        ui->customPlotSecond->graph(0)->addData(key, routeX/30);
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
                                0)->data()->size()), 0);
        lastFpsKey = key;
        frameCount = 0;
    }
}


void MainWindow::on_startButton_clicked()
{
    if(isStartButtonClicked == false){
        isStartButtonClicked = true;
        isStopButtonClicked = false;
        isResetButtonClicked = false;
        qDebug() << "startButton was clicked";
        startTimer();//start timer
        dataTimerFirst.start();
        dataTimerSecond.start();
        scene2dTh->start();

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
    ui->comPortBox->setDisabled(true);
    ui->ipNumberLine->setDisabled(true);
    ui->portButton->setDisabled(true);
    ui->comPortBox->setDisabled(true);
    ui->portNumberLine->setDisabled(true);
}

void MainWindow::on_stopButton_clicked()
{
    if(isStopButtonClicked == false){
        isStartButtonClicked = false;

        stopTimer();//stop timer

        isStopButtonClicked = true;
        onClickedMessage = "q";
        sendData();
        try{
           // socket->close();
        }catch(...){
            qDebug() << "socket close failed" << endl;
        }
        ui->comPortBox->setEnabled(true);
        ui->ipNumberLine->setEnabled(true);
        ui->portButton->setEnabled(true);
        ui->comPortBox->setEnabled(true);
        ui->portNumberLine->setEnabled(true);
        dataTimerFirst.stop();
        dataTimerSecond.stop();
        ui->picLabel->hide();
        isFound = false;
        qDebug() << "stopButton was clicked";
    }
}

void MainWindow::on_portButton_clicked()
{
    QString temp = ui->portNumberLine->text();

    portNumber = temp.toInt();
    qDebug("portNumber: ");
    qDebug() << temp.toLatin1();
    //strcpy(ipNumber,ui->ipNumberLine->text().toLatin1());
	ipNumber = ui->ipNumberLine->text();
    qDebug("ipNumber: ");
    //qDebug() << ipNumber << endl;
    qDebug() << ipNumber.toLatin1() << endl;
	
    QString comPort = ui->comPortBox->currentText();

    qDebug() << comPort.toLatin1();

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
        isFound = false;
        ui->picLabel->hide();
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

