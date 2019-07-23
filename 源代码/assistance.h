#ifndef ASSISTANCE_H
#define ASSISTANCE_H

#include "Enemy.h"

#include <QTimer>
#include <QObject>
#include <QVector>
#include <QGraphicsPixmapItem>

class Assistance : public QObject{
    Q_OBJECT
public:
    Assistance(QObject *parent = 0);
    void enemyCollision();
    QVector <Enemy *> collisions;
    QTimer *timer0;
    QGraphicsPixmapItem *pix0;
    QGraphicsPixmapItem *pix1;
    Enemy *temp2;
    Enemy *temp1;
    bool isok;
public slots:
    void enter();
    void gameover();
    void createItems();
};

#endif // ASSISTANCE_H
