#include "Cloud.h"
#include "Game.h"

#include <QTime>
#include <QtGlobal>
#include <QPainter>
#include <QTransform>
#include <QGraphicsItem>
#include <QPainterPath>
#include <QGraphicsPixmapItem>

extern Game * game;

Cloud::Cloud(QGraphicsItem *parent): QGraphicsPixmapItem(parent){
    pix.load(":/images/cloud0.png");
    setVisible(true);
    setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsFocusable);
    do{direction_x=qrand()%3-1;}while(direction_x==0);
    do{direction_y=qrand()%3-1;}while(direction_y==0);
    direction_x/=2;
    direction_y/=2;
}

QRectF Cloud::boundingRect() const{
    return QRectF(-75/2,-75/2,75,75);
}

void Cloud::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->drawPixmap(-75/2,-75/2,75,75,pix,0,0,64,64);
}

QPainterPath Cloud::shape() const{
    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}

void Cloud::rotate(){
    setTransformOriginPoint(75/2,75/2);
    setRotation(rotation()+2);

    if(pos().x()>=-4500&&pos().x()<=4500&&pos().y()>=-2500&&pos().y()<=2500)
        moveBy(direction_x,direction_y);
    else{
        direction_x=-direction_x;
        direction_y=-direction_y;
        moveBy(direction_x,direction_y);
    }
}
