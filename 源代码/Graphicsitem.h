#ifndef GRAPHICSITEM_H
#define GRAPHICSITEM_H

#include <QObject>
#include <QPointF>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>

class GraphicsItem: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
public:
    GraphicsItem(QGraphicsItem *parent=0);
};

#endif // GRAPHICSITEM_H
