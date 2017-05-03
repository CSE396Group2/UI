#include "mainwindow.h"
#include "ui_mainwindow.h"
/*
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QVBoxLayout>
#include <QMenu>
#include <QAction>
#include <QFileDialog>*/
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
  //  cameraView();
    drawer = new Drawer(this);
    ui->graphicsView->setScene(drawer);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::MyTimer()
{

    // create a timer
    timer = new QTimer(this);

    // setup signal and slot
    connect(timer, SIGNAL(timeout()),
          this, SLOT(MyTimerSlot()));
    // msec
    timer->start(1);
}
void MainWindow::MyTimerSlot()
{

    count++;
    ui->Timer->setText(QString::number((count/100)/60)+":"+QString::number((count/100)%60)+":"+QString::number(count%100));

}
/*void MainWindow::cameraView(){

    mCamera = new QCamera(this);
    mCameraImageCapture = new QCameraImageCapture(mCamera,this);
    mCameraViewfinder = new QCameraViewfinder(this);
    mLayout = new QVBoxLayout;
    mOptionsMenu = new QMenu("Options",this);
    mTurnAction = new QAction("TurnOn",this);
    mDeleteAction = new QAction("TurnOff",this);
    mCaptureAction = new QAction("Capture",this);

    mOptionsMenu->addActions({mTurnAction,mDeleteAction,mCaptureAction});
    ui->optionsPushButton->setMenu(mOptionsMenu);
    mCamera->setViewfinder(mCameraViewfinder);
    mLayout->addWidget(mCameraViewfinder);
    mLayout->setMargin(0);
    //ui->scrollArea->setLayout(mLayout);
    ui->openGLWidget->setLayout(mLayout);
    connect(mTurnAction,&QAction::triggered,[&](){
        mCamera->start();
    });

    connect(mDeleteAction,&QAction::triggered,[&](){
         mCamera->stop();
    });
    connect(mCaptureAction,&QAction::triggered,[&](){
        auto filename = QFileDialog::getSaveFileName(this,"Capture","/","Imagen(*.jpg;jpeg)");

        if(filename.isEmpty()){
            return;
        }

        mCameraImageCapture->setCaptureDestination(
                    QCameraImageCapture::CaptureToFile);
        QImageEncoderSettings imageEncoderSettings;
        imageEncoderSettings.setCodec("image/jpeg");
        imageEncoderSettings.setResolution(1600,1200);
        mCameraImageCapture->setEncodingSettings(imageEncoderSettings);
        mCamera->setCaptureMode(QCamera::CaptureStillImage);
        mCamera->start();
        mCamera->searchAndLock();
        mCameraImageCapture->capture(filename);
        mCamera->unlock();
    });
}*/
void MainWindow::startServer(){
    qDebug() << "Server started" ;
}

void MainWindow::on_startButton_clicked()
{
    if(isStartButtonClicked == false){
        isStartButtonClicked = true;
        isStopButtonClicked = false;
        strcpy(onClickedMessage,"message");
        //MyTimer myTimer();
        qDebug() << "startButton was clicked";
        MyTimer();
        ui->coordinatBrowser->append("gurol");

    }
}

void MainWindow::on_stopButton_clicked()
{
    if(isStopButtonClicked == false){
        isStartButtonClicked = false;
         ui->Timer->setText("Stoped");
        isStopButtonClicked = true;
        qDebug() << "stopButton was clicked";
    }
}
