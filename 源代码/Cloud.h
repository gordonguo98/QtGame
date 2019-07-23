#ifndef CLOUD_H
#define CLOUD_H

#include <QRectF>
#include <QPixmap>
#include <QGraphicsItem>
#include <QGraphicsObject>

class Cloud:public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
public:
    Cloud(QGraphicsItem *parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    QPixmap pix;
public slots:
    void rotate();
private:
    qreal direction_x;
    qreal direction_y;
};

#endif // CLOUD_H
