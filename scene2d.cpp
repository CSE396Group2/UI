#include "scene2d.h"
#include <QGraphicsSceneMouseEvent>

Scene2d::Scene2d(QObject *parent):QGraphicsScene(parent)
{
    this->addRect(LEFT_BORDER, TOP_BORDER,RIGHT_BORDER-LEFT_BORDER,BOTTOM_BORDER-TOP_BORDER);
    this->addRect(LEFT_BORDER, TOP_BORDER, RIGHT_BORDER-LEFT_BORDER, BOTTOM_BORDER-TOP_BORDER);
}
void Scene2d::draw(){
    this->clear();
    this->addRect(LEFT_BORDER, TOP_BORDER,RIGHT_BORDER-LEFT_BORDER,BOTTOM_BORDER-TOP_BORDER);

    QPen camPen(Qt::red);
    QBrush camBrush(Qt::blue);

    qreal x = coordinateToPositionX(getBoardX());
    qreal y = coordinateToPositionY(getBoardY());
    this->addEllipse(collisionX(x)-BALL_RADIUS,collisionY(y)-BALL_RADIUS,2*BALL_RADIUS,2*BALL_RADIUS,camPen,camBrush);
    QString coordinates = QString("X: ") + QString::number(getBoardX()) + QString(" Y: ") + QString::number(getBoardY());
    this->addText(coordinates);
}

int Scene2d::getBoardX(){
    return boardX;
}

int Scene2d::getBoardY(){
    return boardY;
}

qreal Scene2d::coordinateToPositionX(int x)
{
    const qreal X_MAX = 1000;
    //return (x-LEFT_BORDER)*(RIGHT_BORDER-LEFT_BORDER)/X_MAX;
    return LEFT_BORDER + x*(RIGHT_BORDER-LEFT_BORDER)/X_MAX;
}

qreal Scene2d::coordinateToPositionY(int y)
{
    const qreal Y_MAX = 1000;
    //return (y-TOP_BORDER)*(BOTTOM_BORDER-TOP_BORDER)/Y_MAX;
    return TOP_BORDER + y*(BOTTOM_BORDER-TOP_BORDER)/Y_MAX;
}

qreal Scene2d::collisionX(qreal x){
    if(x-BALL_RADIUS < LEFT_BORDER)
        return LEFT_BORDER + BALL_RADIUS;

    if(x+BALL_RADIUS > RIGHT_BORDER)
        return RIGHT_BORDER - BALL_RADIUS;

    return x;
}
qreal Scene2d::collisionY(qreal y){
    if(y-BALL_RADIUS < TOP_BORDER)
        return TOP_BORDER + BALL_RADIUS;

    if(y+BALL_RADIUS > BOTTOM_BORDER)
        return BOTTOM_BORDER - BALL_RADIUS;

    return y;
}
