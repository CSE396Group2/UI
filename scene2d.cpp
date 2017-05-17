#include "scene2d.h"
#include <QGraphicsSceneMouseEvent>


Scene2d::Scene2d(QObject *parent):QGraphicsScene(parent)
{
    addRuller();
    this->addRect(LEFT_BORDER, TOP_BORDER,RIGHT_BORDER-LEFT_BORDER,BOTTOM_BORDER-TOP_BORDER);

}
void Scene2d::draw(){
    this->clear();
    addRuller();
    this->addRect(LEFT_BORDER, TOP_BORDER,RIGHT_BORDER-LEFT_BORDER,BOTTOM_BORDER-TOP_BORDER);

    QPen camPen(Qt::red);
    QBrush camBrush(Qt::red);

    qreal x = getBoardX(); // long side
    qreal y = getBoardY(); // short side
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
    const qreal X_MAX = 660; //long side
    //return (x-LEFT_BORDER)*(RIGHT_BORDER-LEFT_BORDER)/X_MAX;
    return LEFT_BORDER + x*(RIGHT_BORDER-LEFT_BORDER)/X_MAX;
}

qreal Scene2d::coordinateToPositionY(int y)
{
    const qreal Y_MAX = 474; //short side
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
void Scene2d::addRuller(){
    QPen linePen(Qt::blue);
    for (int var = 0; var < RIGHT_BORDER-29; ++var) {
        if(var % 50 == 0){
            //this->addText();
            this->addLine(var+LEFT_BORDER,0,var+LEFT_BORDER,30,linePen);
        }
        else if(var % 25 == 0)
            this->addLine(var+LEFT_BORDER,0,var+LEFT_BORDER,20,linePen);
        else if(var % 5 == 0)
            this->addLine(var+LEFT_BORDER,0,var+LEFT_BORDER,10,linePen);
    }
    for(int i = 0; i < BOTTOM_BORDER-29; ++i){
        if(i % 50 == 0)
            this->addLine(0,i+TOP_BORDER,30,i+TOP_BORDER,linePen);
        else if(i % 25 == 0)
            this->addLine(0,i+TOP_BORDER,20,i+TOP_BORDER,linePen);
        else if(i % 5 == 0)
            this->addLine(0,i+TOP_BORDER,10,i+TOP_BORDER,linePen);

    }
}

void Scene2d::setBoard(int x, int y)
{
    boardX = x;
    boardY = y;
}
