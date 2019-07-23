#include "Assistance.h"
#include "Game.h"
#include "Enemy.h"
#include "Melee.h"

#include <QPen>
#include <QSize>
#include <QTimer>
#include <QPixmap>
#include <QObject>
#include <typeinfo>
#include <QResource>
#include <QGraphicsPixmapItem>

extern Game * game;

qreal a_distance(qreal x1,qreal y1,qreal x2,qreal y2){
    return pow(pow(x1-x2,2)+pow(y1-y2,2),0.5);
}

Assistance::Assistance(QObject *parent) : QObject(parent){
    timer0=new QTimer();
    pix0=new QGraphicsPixmapItem(QPixmap(":/images/welcome0.png"));
    pix0->setOpacity(1);
    pix1=new QGraphicsPixmapItem(QPixmap(":/images/welcome1.png"));
    pix1->setOpacity(1);
    //contain the enemies which are colliding
    collisions.resize(0);
    isok=false;
}

void Assistance::enemyCollision(){
    //judge the collision results
    for(int i=0,n=collisions.size();i<n;i++){
        if(collisions[i]!=NULL&&collisions[i]->scene()!=NULL&&collisions[i]->isCollided==true){
            temp1=collisions[i];
            int col=temp1->col_id;
            temp2=game->enemy[col];
            qreal d=a_distance(temp1->pos().x(),temp1->pos().y(),
                               temp2->pos().x(),temp2->pos().y());
            if(temp1->sx<temp2->sx){
                if(2*d<=temp2->sx&&game->ratio*temp1->sx<=temp2->sx){
                    temp2->increase(temp1->sx);
                    temp2->isCollided=false;
                    temp1->isCollided=false;
                    temp1->sx=20;
                    temp1->sy=20;
                    temp1->weight=0;
                    temp1->setPos(qrand()%9000-4500,qrand()%5000-2500);
                    if((temp1->childItems()).size()>1){
                        if(typeid(*((temp1->childItems())[1]))==typeid(Shield)){
                            game->shieldamount-=1;
                            temp1->hasshield=false;
                            temp1->collideWithShield=false;
                        }
                        else if(typeid(*((temp1->childItems())[1]))==typeid(Ice)){
                            game->iceamount-=1;
                            temp1->hasice=false;
                        }
                        else if(typeid(*((temp1->childItems())[1]))==typeid(Speedup)){
                            game->speedupamount-=1;
                            temp1->hasspeedup=false;
                        }
                        else if(typeid(*((temp1->childItems())[1]))==typeid(Liquid)){
                            game->liquidamount-=1;
                            temp1->hasliquid=false;
                        }
                        delete ((temp1->childItems())[1]);
                    }
                }
            }
            else if(temp1->sx>=temp2->sx){
                if(2*d<=temp1->sx&&game->ratio*temp2->sx<=temp1->sx){
                    temp1->increase(temp2->sx);
                    temp1->isCollided=false;
                    temp2->isCollided=false;
                    temp2->sx=20;
                    temp2->sy=20;
                    temp2->weight=0;
                    temp2->setPos(qrand()%9000-4500,qrand()%5000-2500);
                    if((temp2->childItems()).size()>1){
                        if(typeid(*((temp2->childItems())[1]))==typeid(Shield)){
                            game->shieldamount-=1;
                            temp2->hasshield=false;
                            temp2->collideWithShield=false;
                        }
                        else if(typeid(*((temp2->childItems())[1]))==typeid(Ice)){
                            game->iceamount-=1;
                            temp2->hasice=false;
                        }
                        else if(typeid(*((temp2->childItems())[1]))==typeid(Speedup)){
                            game->speedupamount-=1;
                            temp2->hasspeedup=false;
                        }
                        else if(typeid(*((temp2->childItems())[1]))==typeid(Liquid)){
                            game->liquidamount-=1;
                            temp2->hasliquid=false;
                        }
                        delete ((temp2->childItems())[1]);
                    }
                }
            }
        }
    }
    collisions.clear();
}

void Assistance::enter(){
    if(isok==false){
        if(pix0->opacity()<=1&&pix0->opacity()>0)
            pix0->setOpacity(pix0->opacity()-0.01);
        else{
            if(pix0->scene()!=NULL){
                game->scene->removeItem(pix0);
                delete pix0;
            }
            pix1->setVisible(true);
            if(pix1->opacity()<=1&&pix1->opacity()>0){
                pix1->setOpacity(pix1->opacity()-0.01);
            }
            else{
                if(pix1->scene()!=NULL){
                    game->scene->removeItem(pix1);
                    delete pix1;
                }
                isok=true;
            }
        }
    }
    else{
        timer0->stop();
        delete timer0;
        game->control();
    }
}

void Assistance::gameover(){
    delete game->timer1;
    delete game->timer2;
    delete game->timer3;
    for(int i=0;i<game->timer4.size();i++)
        delete game->timer4[i];
    for(int i=0;i<game->timer5.size();i++)
        delete game->timer5[i];
    delete game->music;
    delete game->timecount;
    delete game->timingcount;
    delete game->timing;
    game->centerOn(0,0);
    game->billboard->showresult();
    game->player->shooting->setPos(700,300);
    game->player->shooting->setVisible(false);
    game->scene->update();
}

void Assistance::createItems(){
    if(game->liquidamount<5){
        Liquid *lqd=new Liquid;
        game->scene->addItem(lqd);
        lqd->setPos(qrand()%9000-4500,qrand()%5000-2500);
        game->liquidamount+=1;
    }
    if(game->iceamount<5){
        Ice *ice=new Ice;
        game->scene->addItem(ice);
        ice->setPos(qrand()%9000-4500,qrand()%5000-2500);
        game->iceamount+=1;
    }
    if(game->shieldamount<5){
        Shield *sld=new Shield;
        game->scene->addItem(sld);
        sld->setPos(qrand()%9000-4500,qrand()%5000-2500);
        game->shieldamount+=1;
    }
    if(game->speedupamount<5){
        Speedup *spd=new Speedup;
        game->scene->addItem(spd);
        spd->setPos(qrand()%9000-4500,qrand()%5000-2500);
        game->speedupamount+=1;
    }
}
