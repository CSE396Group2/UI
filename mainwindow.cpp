#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    drawer = new Drawer(this);
    ui->graphicsView->setScene(drawer);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    connectionTh = new ConnectionThread(this);
    connect(connectionTh,SIGNAL(startConnection()),this,SLOT(isConnect()),Qt::DirectConnection);
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

void MainWindow::on_startButton_clicked()
{
    if(isStartButtonClicked == false){
        isStartButtonClicked = true;
        isStopButtonClicked = false;
        strcpy(onClickedMessage,"message");       
        qDebug() << "startButton was clicked";

        startTimer();//start timer
        ui->coordinatBrowser->append("gurol");
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
        qDebug() << "stopButton was clicked";
    }
}

void MainWindow::isConnect()
{
//    qDebug("geldim co");
//    if(!server->listen(QHostAddress::Any,portNumber)){
//        qDebug("connection error");
//    }
//    else{
//        qDebug("connection start");
//    }
//    qDebug() << portNumber;
//    while(socket == NULL){
//        if(server->hasPendingConnections()){
//            qDebug("has connection");
//            QTcpSocket *socket = server->nextPendingConnection();
//            while(socket != NULL){
//                socket->write("start");
//                socket->flush();
//                socket->waitForBytesWritten(30);
//                socket->waitForReadyRead(30);

//                QByteArray array = socket->readAll();
//                qDebug(array);
//            }
//        }
//    }
    QHostAddress hostAddr(ipNumber);
    socket = new QTcpSocket(this);
    socket->connectToHost(hostAddr,portNumber);

    if(socket->waitForConnected(5000)){
        while(!isStopButtonClicked){
            socket->write("selam");
            socket->waitForBytesWritten(3000);
        }
        qDebug("stop press");
        socket->write("q");
        socket->waitForBytesWritten(3000);
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
