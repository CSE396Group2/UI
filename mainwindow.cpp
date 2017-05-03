#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    drawer = new Drawer(this);
    ui->graphicsView->setScene(drawer);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
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
    timer->start(1);
}
void MainWindow::counterTimer()
{
    countTime++;
    ui->timerLabel->setText(QString::number((countTime/100)/60)+":"+QString::number((countTime/100)%60)+":"+QString::number(countTime%100));

}
void MainWindow::stopTimer()
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
