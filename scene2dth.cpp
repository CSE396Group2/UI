#include "scene2dth.h"

Scene2dTh::Scene2dTh(QObject *parent) : QThread(parent)
{

}

void Scene2dTh::run()
{
    emit update2DScene();
}
