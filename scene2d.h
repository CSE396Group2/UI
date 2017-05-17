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
    const qreal LEFT_BORDER = 30; //x
    const qreal RIGHT_BORDER = 660; //x
    const qreal TOP_BORDER = 30; //y
    const qreal BOTTOM_BORDER = 474; //y
    const qreal BALL_RADIUS = 10;
private:
     qreal collisionX(qreal x);
     qreal collisionY(qreal y);

     qreal coordinateToPositionX(int x);
     qreal coordinateToPositionY(int y);
     void addRuller();
     int boardX;
     int boardY;
};

#endif // SCENE2D_H
