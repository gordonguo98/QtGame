#ifndef BUBBLE_H
#define BUBBLE_H

#include <QObject>
#include <QPointF>
#include <QGraphicsEllipseItem>

class Bubble:public QObject, public QGraphicsEllipseItem{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
public:
    Bubble(QGraphicsItem *parent=0);
};

#endif // BUBBLE_H
