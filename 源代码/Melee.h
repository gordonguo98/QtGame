#ifndef MELEE_H
#define MELEE_H

#include "Enemy.h"
#include "Player.h"

#include <QObject>
#include <QPixmap>
#include <QDebug>
#include <QList>
#include <QTimer>
#include <QtGlobal>
#include <typeinfo>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>

class Blackhole : public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
public:
    Blackhole(QGraphicsItem *parent=0): QGraphicsPixmapItem(parent){
        setPixmap(QPixmap(":/images/blackhole.png"));
    }
    int id;
};

class Shield : public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
public:
    Shield(QGraphicsItem *parent=0): QGraphicsPixmapItem(parent){
        setPixmap(QPixmap(":/images/shield.png"));
    }
    int state=0;
public slots:
    void endlife(){
        delete this;
    }
};

class Ice : public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
public:
    Ice(QGraphicsItem *parent=0): QGraphicsPixmapItem(parent){
        setPixmap(QPixmap(":/images/ice.png"));
    }
public slots:
    void detect(){
        QList<QGraphicsItem *> colliding_items = collidingItems();
        for (int i = 0, n = colliding_items.size(); i < n; ++i){
            if (typeid(*(colliding_items[i])) == typeid(Enemy)){
                Enemy *temp=dynamic_cast<Enemy *>(colliding_items[i]);
                temp->isactive=false;
                QTimer *timertemp=new QTimer;
                QObject::connect(timertemp,SIGNAL(timeout()),temp,SLOT(becomeActive()));
                QObject::connect(timertemp,SIGNAL(timeout()),timertemp,SLOT(deleteLater()));
                timertemp->start(5000);
                delete this;
            }
            else if(typeid(*(colliding_items[i])) == typeid(Player)){
                Player *temp=dynamic_cast<Player *>(colliding_items[i]);
                temp->isactive=false;
                QTimer *timertemp=new QTimer;
                QObject::connect(timertemp,SIGNAL(timeout()),temp,SLOT(becomeActive()));
                QObject::connect(timertemp,SIGNAL(timeout()),timertemp,SLOT(deleteLater()));
                timertemp->start(5000);
                delete this;
            }
        }
    }//detect whether colliding with items
};

class Speedup : public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
public:
    Speedup(QGraphicsItem *parent=0): QGraphicsPixmapItem(parent){
        setPixmap(QPixmap(":/images/speedup.png"));
    }
};

class Liquid : public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
public:
    Liquid(QGraphicsItem *parent=0): QGraphicsPixmapItem(parent){
        setPixmap(QPixmap(":/images/liquid.png"));
    }
public slots:
    void detect(){
        QList<QGraphicsItem *> colliding_items = collidingItems();
        for (int i = 0, n = colliding_items.size(); i < n; ++i){
            if (typeid(*(colliding_items[i])) == typeid(Enemy)){
                Enemy *temp=dynamic_cast<Enemy *>(colliding_items[i]);
                for(int i=0;i<8;i++)
                    temp->createbubble((qrand()%629)/100);
                delete this;
            }
            else if(typeid(*(colliding_items[i])) == typeid(Player)){
                Player *temp=dynamic_cast<Player *>(colliding_items[i]);
                for(int i=0;i<8;i++)
                    temp->createBubble((qrand()%629)/100);
                delete this;
            }
        }
    }//detect whether colliding with items
};

#endif // MELEE_H
