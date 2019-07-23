#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "Enemy.h"

#include <QObject>
#include <QVector>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>

class Billboard:public QGraphicsTextItem{
    Q_OBJECT
public:
    Billboard(QGraphicsItem * parent=0);
    void placeBillboard();
    void showresult();
    QVector <Enemy*> billboard;
    QVector <QGraphicsTextItem*> text;
    QGraphicsPixmapItem *result;
public slots:
    void updateBillboard();
};

#endif
