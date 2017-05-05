#include "coordinatebrowserth.h"


CoordinateBrowserTh::CoordinateBrowserTh(QObject *parent) :
    QThread(parent)
{

}

void CoordinateBrowserTh::run()
{
    emit updateBrowser();
}
