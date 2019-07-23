#ifndef MYITEM_H
#define MYITEM_H

#include <QTimer>
#include <QObject>
#include <QPixmap>
#include <QGraphicsPixmapItem>

class Myitem : public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
public:
    Myitem(QGraphicsItem *parent = 0);
    void start();
public slots:
    void changeDirection();
    void move();

private:
    QTimer *timer1;
    QTimer *timer2;
    qreal direction;
    qreal v;
};
#endif // MYITEM_H
