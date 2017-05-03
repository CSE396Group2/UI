#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTimer>
/*
class QCamera;
class QCameraViewfinder;
class QCameraImageCapture;
class QVBoxLayout;
class QMenu;
class QAction;
*/
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
  /*  QCamera *mCamera;
    QCameraViewfinder *mCameraViewfinder;
    QCameraImageCapture *mCameraImageCapture;
    QVBoxLayout *mLayout;
    QMenu *mOptionsMenu;
    QAction *mTurnAction;
    QAction *mDeleteAction;
    QAction *mCaptureAction;
*/
    QTimer *timer;
    int count=0;
    char onClickedMessage[64];
    bool isStartButtonClicked = false;
    bool isStopButtonClicked = false;

private slots:

    void MyTimer();
    void MyTimerSlot();
    //void cameraView();
    void on_startButton_clicked();
    void startServer();
    void on_stopButton_clicked();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
