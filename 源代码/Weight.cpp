#include "Weight.h"
#include "Game.h"

#include <QFont>

extern Game * game;

Weight::Weight(QGraphicsItem *parent): QGraphicsTextItem(parent){
    // draw the text
    setPlainText(QString("Weight: ") + QString::number(game->player->playerWeight)+QString(" g"));
    setDefaultTextColor(Qt::white);
    setFont(QFont("times",16));
}

void Weight::showWeight(){
    setPlainText(QString("Weight: ") + QString::number(game->player->playerWeight)+QString(" g"));
}
