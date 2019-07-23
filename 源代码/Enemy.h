#ifndef ENEMY_H
#define ENEMY_H

#include "Player.h"
#include "Bubble.h"
#include "Cloud.h"

#include <QTime>
#include <QRectF>
#include <QVector>
#include <QString>
#include <QObject>
#include <QPixmap>
#include <QPainterPath>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>

class Enemy: public QObject,public QGraphicsPixmapItem{
    Q_OBJECT
public:
    Enemy(QGraphicsItem * parent=0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    void createRect();
    void judge();
    void increase(qreal);
    void decrease(qreal);
    void reset();
    void createbubble(qreal);
    QPixmap pix;
    qreal sx;
    qreal sy;
    int weight;
    QString name;
    int id;//identity of enemy
    int col_id;//identity of the enemy collided with it
    bool isCollided;
    Bubble *bubble;
    QPropertyAnimation *a;
    bool isactive;
    bool shoot;
    bool hasshield;
    bool hasice;
    bool hasspeedup;
    bool hasliquid;
    bool collideWithShield;
    bool isfast;
    bool isinvisible;
    QPropertyAnimation *ani;
public slots:
    void move();
    void updateRect();
    void becomeActive();
    void becomefast();
    void becomeInvisible();
private:
    void calculate(QList<QGraphicsItem *> &list);
    QRectF *rect_lu;
    qreal rect_x;
    qreal rect_y;
    qreal direction;
    qreal v;
    QVector <QString*> pixaddress;
    QGraphicsTextItem nameItem;
    Player *temp;
    Enemy *tempp;
    Cloud *tempc;
};

#endif // ENEMY_H
