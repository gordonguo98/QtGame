#include "Myitem.h"

#include <QTime>
#include <QTimer>
#include <QtMath>
#include <QtGlobal>

Myitem::Myitem(QGraphicsItem *parent) : QGraphicsPixmapItem(parent){
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    timer1=new QTimer(this);
    timer2=new QTimer(this);
    connect(timer1,SIGNAL(timeout()),this,SLOT(move()));
    connect(timer2,SIGNAL(timeout()),this,SLOT(changeDirection()));
    v=15;
    direction=(qrand()%629)/100;
}

void Myitem::changeDirection(){
    direction=(qrand()%629)/100;
}

void Myitem::move(){
    if(pos().x()>=-950&&pos().x()<=950&&pos().y()>=-550&&pos().y()<=550)
        moveBy((v*qCos(direction)),-(v*qSin(direction)));
    else{
        qreal k=(-pos().y())/(-pos().x());
        if(pos().x()<=0&&pos().y()<=0)
            direction=6.28-qAtan(k);
        else if(pos().x()<=0&&pos().y()>=0)
            direction=-qAtan(k);
        else
            direction=3.14-qAtan(k);
        moveBy((v*qCos(direction)),-(v*qSin(direction)));
    }
}

void Myitem::start(){
    timer1->start(100);
    timer2->start(4000);
}
