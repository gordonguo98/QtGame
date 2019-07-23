#include "Game.h"
#include "Billboard.h"

#include <QRgb>
#include <QFont>
#include <QColor>
#include <QVector>
#include <QGraphicsTextItem>

extern Game * game;

void sort(QVector<Enemy*> &vec, int low, int high){
    if (low < high){
        int l = low;
        int r = high;
        int key = vec[l]->weight;
        Enemy *temp=vec[l];

        while (l < r){
            while (l < r&&key >= vec[r]->weight)
                --r;
            vec[l] = vec[r];
            while (l < r&&key <= vec[l]->weight)
                ++l;
            vec[r] = vec[l];
        }
        vec[l] = temp;

        sort(vec, low, l-1);
        sort(vec, r + 1, high);
    }
}

Billboard::Billboard(QGraphicsItem *parent): QGraphicsTextItem(parent){
    //get the list of items
    billboard=game->enemy;
    sort(billboard,0,billboard.size()-1);
    text.resize(billboard.size()+1);
    for(int i=0;i<text.size();i++){
        text[i]=new QGraphicsTextItem;
        text[i]->setDefaultTextColor(Qt::white);
        text[i]->setFont(QFont("times",12));
    }
}

void Billboard::placeBillboard(){
    int insertion=0;
    for(int i=0;i<billboard.size();i++){
        if(game->player->playerWeight>=billboard[i]->weight){
            insertion=i;
            break;
        }
        insertion=billboard.size();
    }
    for(int i=0;i<insertion;i++){
        text[i]->setPlainText(QString::number(i+1)+QString(": ")+
                              billboard[i]->name+QString(" ")+QString::number(billboard[i]->weight)+QString(" g"));
        game->scene->addItem(text[i]);
        text[i]->setZValue(200);
        text[i]->setPos(game->mapToScene(1550,10+i*25));
    }
    text[insertion]->setPlainText(QString::number(insertion+1)+QString(": ")+
                                  game->player->name+QString(" ")+QString::number(game->player->playerWeight)+QString(" g"));
    game->scene->addItem(text[insertion]);
    text[insertion]->setZValue(200);
    text[insertion]->setPos(game->mapToScene(1550,10+insertion*25));
    if(insertion!=billboard.size()){
        for(int i=insertion+1;i<billboard.size();i++){
            text[i]->setPlainText(QString::number(i+1)+QString(": ")+
                                  billboard[i-1]->name+QString(" ")+QString::number(billboard[i-1]->weight)+QString(" g"));
            game->scene->addItem(text[i]);
            text[i]->setZValue(200);
            text[i]->setPos(game->mapToScene(1550,10+i*25));
        }
    }
}

void Billboard::showresult(){
    game->interface=game->GAMEOVER;

    result=new QGraphicsPixmapItem;
    result->setPixmap(QPixmap(":/images/billboard.png"));
    game->scene->addItem(result);
    result->setPos(-200,-250);
    result->setZValue(19);

    int insertion=0;
    for(int i=0;i<billboard.size();i++){
        if(game->player->playerWeight>=billboard[i]->weight){
            insertion=i;
            break;
        }
        insertion=billboard.size();
    }
    if(insertion<8){
        for(int i=0;i<insertion;i++){
            text[i]->setPlainText(billboard[i]->name+QString("    ")+QString::number(billboard[i]->weight)+QString(" g"));
            text[i]->setPos(-140,-190+i*50);
        }
        text[insertion]->setPlainText(game->player->name+QString("    ")+QString::number(game->player->playerWeight)+QString(" g"));
        text[insertion]->setPos(-140,-190+insertion*50);
        if(insertion!=7){
            for(int i=insertion+1;i<billboard.size();i++){
                if(i<8){
                    text[i]->setPlainText(billboard[i-1]->name+QString("    ")+QString::number(billboard[i-1]->weight)+QString(" g"));
                    text[i]->setPos(-140,-190+i*50);
                }
            }
        }
    }
    else{
        for(int i=0;i<billboard.size()&&i<8;i++){
            text[i]->setPlainText(billboard[i]->name+QString("    ")+QString::number(billboard[i]->weight)+QString(" g"));
            text[i]->setPos(-140,-190+i*50);
        }
    }
}

void Billboard::updateBillboard(){
    sort(billboard,0,billboard.size()-1);
    for(int i=0;i<text.size();i++){
        if(text[i]->scene()!=NULL){
            game->scene->removeItem(text[i]);
        }
    }
    placeBillboard();
}
