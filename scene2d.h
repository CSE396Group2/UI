#ifndef SCENE2D_H
#define SCENE2D_H

#include <QGraphicsScene>
#include <QtGui>
#include <QtCore>

class Scene2d:public QGraphicsScene
{
public:
    Scene2d(QObject *parent = 0);
    QPainter painter;
    void draw();
    int getBoardX();
    int getBoardY();
    void setBoard(int x, int y);
    const qreal LEFT_BORDER = 0; //x
    const qreal RIGHT_BORDER = 630; //x
    const qreal TOP_BORDER = 0; //y
    const qreal BOTTOM_BORDER = 444; //y
    const qreal BALL_RADIUS = 10;
private:
     qreal collisionX(qreal x);
     qreal collisionY(qreal y);

     qreal coordinateToPositionX(int x);
     qreal coordinateToPositionY(int y);

     int boardX;
     int boardY;
};

#endif // SCENE2D_H
