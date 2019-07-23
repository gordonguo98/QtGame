#include "Helper.h"
#include "Game.h"

#include <QPixmap>

extern Game *game;

Helper::Helper(QObject *parent): QObject(parent){

}

void Helper::resetKey(){
    game->lastkey=-1;
}

void Helper::timing(){
    game->time++;
    int hundrednum=int(((game->time)/100)%10);
    int tennum=int(((game->time)/10)%10);
    int onenum=game->time%10;
    game->hundred->setPixmap(QPixmap(game->numbers[hundrednum]));
    game->ten->setPixmap(QPixmap(game->numbers[tennum]));
    game->one->setPixmap(QPixmap(game->numbers[onenum]));
}
