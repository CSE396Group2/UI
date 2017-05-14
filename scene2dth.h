#ifndef SCENE2DTH_H
#define SCENE2DTH_H

#include <QThread>
#include <QtCore>

class Scene2dTh : public QThread
{
    Q_OBJECT
public:
    explicit Scene2dTh(QObject *parent = 0);
    void run();
signals:
    void update2DScene();
public slots:

};

#endif // SCENE2DTH_H
