#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startServer(){
    qDebug() << "Server started" ;
}

void MainWindow::on_startButton_clicked(bool checked)
{
    if(isStartButtonClicked == false){
        isStartButtonClicked = true;
        isStopButtonClicked = false;
        strcpy(onClickedMessage,"message");
        startServer();
        qDebug() << "startButton was clicked";
    }
}




void MainWindow::on_stopButton_clicked(bool checked)
{
    if(isStopButtonClicked == false){
        isStartButtonClicked = false;
        isStopButtonClicked = true;
        qDebug() << "stopButton was clicked";

    }

}
