#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene2d = new Scene2d(this);
    ui->graphicsView->setScene(scene2d);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    connectionTh = new ConnectionThread(this);
    socket = new QTcpSocket(this);
    connect(connectionTh,SIGNAL(startConnection()),this,SLOT(isConnect()),Qt::DirectConnection);
    connect(&timer2d, SIGNAL(timeout()), this, SLOT(update2DCoordinates()));
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
    socket->connectToHost(hostAddr,portNumber);
    coorBrowTh = new CoordinateBrowserTh();
    connect(coorBrowTh,SIGNAL(updateBrowser()),this,SLOT(updateBrow()));
    if(socket->waitForConnected(5000)){
        timer2d.start();
        timer2d.setInterval(17);
        socket->write(onClickedMessage.toLatin1());
        socket->waitForBytesWritten(3000);

        onClickedMessage = QString::number(comPortNumber);

        socket->write(onClickedMessage.toLatin1());
        socket->waitForBytesWritten(100);

        while(!isStopButtonClicked){
            socket->waitForReadyRead(100);
            socket->bytesAvailable();
            readData = socket->readAll();
            mutex.lock();
            scene2d->setBoard(123,300);
            coorBrowTh->start();
            //qDebug()<< "X:"+QString::number(qrand()%200)+"\tY:"+QString::number(qrand()%200) ;
            mutex.unlock();
            qDebug() << readData;
        }
    }
}

void MainWindow::sendData(){
    socket->write(onClickedMessage.toLatin1());
    socket->waitForBytesWritten(3000);
}

void MainWindow::updateBrow()
{
    ui->coordinatBrowser->append("X:"+QString::number(qrand()%200)+"\tY:"+QString::number(qrand()%200));
}

void MainWindow::on_startButton_clicked()
{
    if(isStartButtonClicked == false){
        isStartButtonClicked = true;
        isStopButtonClicked = false;
        onClickedMessage = "x";
        qDebug() << "startButton was clicked";
        startTimer();//start timer
       // ui->coordinatBrowser->append("gurol");
        //ui->coordinatBrowser->append("X:"+QString::number(qrand()%200)+"\tY:"+QString::number(qrand()%200));
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
        socket->close();
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
         //  isStopButtonClicked = false;
          // isStartButtonClicked = false;

        restartTimer();//start timer
        isResetButtonClicked = false;
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
