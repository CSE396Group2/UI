#ifndef COORDINATEBROWSERTH_H
#define COORDINATEBROWSERTH_H

#include <QThread>
#include <QtCore>

class CoordinateBrowserTh : public QThread
{
    Q_OBJECT
public:
    explicit CoordinateBrowserTh(QObject *parent = 0);
    void run();
signals:
    void updateBrowser();
public slots:

};

#endif // COORDINATEBROWSERTH_H
