#include "Timecounter.h"
#include "Game.h"

extern Game * game;

TimeCounter::TimeCounter(QGraphicsItem * parent):QGraphicsTextItem(parent){
    setPlainText(QString("Time: ") + QString::number(game->ti_l*60)+QString(" s"));
    setDefaultTextColor(Qt::green);
    setFont(QFont("times",16));
    count=0;
}

void TimeCounter::showtime(){
    count++;
    setPlainText(QString("Time: ") + QString::number(game->ti_l*60-count)+QString(" s"));
}
