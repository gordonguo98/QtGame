#ifndef PLAYER_H
#define PLAYER_H

#include "Bubble.h"

#include <QRectF>
#include <QObject>
#include <QString>
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include <QMediaPlayer>
#include <QGraphicsItem>
#include <QPropertyAnimation>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>

class Player:public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
public:
    Player(QGraphicsItem * parent=0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    void setRotate(qreal);
    void increase(qreal);
    void decrease(qreal);
    void reset();
    void createBubble(qreal);
    QPropertyAnimation *a;
    bool shoot;
    bool hasshield;
    bool hasice;
    bool hasspeedup;
    bool hasliquid;
    bool isinvisible;
    bool collideWithShield;
    bool isfast;
    bool isactive;
    QPixmap pix;
    qreal size_x;
    qreal size_y;
    int foodamount;
    int foodstaramount;
    int playerWeight;
    QString name;
    qreal v;
    QMediaPlayer *eatfood;
    QMediaPlayer *eatfoodstar;
    QMediaPlayer *eatcloud;
    QGraphicsPixmapItem *shooting;
    qreal rotate_angle;
public slots:
    void createfood();
    void move();
    void playmusic();
    void updateView();
    void becomeInvisible();
    void becomeFast();
    void becomeActive();
private:
    qreal last;
    qreal x0;
    qreal y0;
    Bubble * bubble;
    QGraphicsEllipseItem * food;
    QGraphicsPixmapItem * foodstar;
    QGraphicsPixmapItem * divide;
    QGraphicsTextItem nameItem;
    qreal offset_x;
    qreal offset_y;
};

#endif // PLAYER_H
