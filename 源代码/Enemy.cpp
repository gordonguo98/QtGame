#include "Enemy.h"
#include "Game.h"
#include "Cloud.h"
#include "Player.h"
#include "Melee.h"
#include "Weight.h"

#include <QTime>
#include <QDebug>
#include <QPoint>
#include <QRectF>
#include <QTimer>
#include <QtMath>
#include <QObject>
#include <QPointF>
#include <QtGlobal>
#include <typeinfo>
#include <stdlib.h>
#include <QtGlobal>
#include <QPainterPath>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

extern Game * game;

qreal Distance(qreal x1,qreal y1,qreal x2,qreal y2){
    return pow(pow(x1-x2,2)+pow(y1-y2,2),0.5);
}

qreal Angle(qreal x1, qreal y1, qreal x2, qreal y2){
    qreal k=(y2-y1)/(x2-x1);
    if(x1<=x2&&y1<=y2)
        return 6.28-qAtan(k);
    else if(x1<=x2&&y1>=y2)
        return -qAtan(k);
    else
        return 3.14-qAtan(k);
}

Enemy::Enemy(QGraphicsItem *parent): QGraphicsPixmapItem(parent){
    weight=0;
    sx=qrand()%300+20;
    sy=sx;
    rect_x=10*sx;
    rect_y=rect_x;
    v=20-0.025*sx;
    pixaddress.resize(7);
    pixaddress[0]=new QString(":/images/enemy0.png");
    pixaddress[1]=new QString(":/images/enemy1.png");
    pixaddress[2]=new QString(":/images/enemy2.png");
    pixaddress[3]=new QString(":/images/enemy3.png");
    pixaddress[4]=new QString(":/images/enemy4.png");
    pixaddress[5]=new QString(":/images/enemy5.png");
    pixaddress[6]=new QString(":/images/enemy6.png");
    pix.load(*(pixaddress[qrand()%7]));
    setVisible(true);
    setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsFocusable);
    direction=(qrand()%629)/100;
    int i;
    do{
        i=qrand()%30;
    }while(game->names[i].notUsed==0);
    name=game->names[i].name;
    game->names[i].notUsed=0;
    nameItem.setPlainText(name);
    nameItem.setDefaultTextColor(QColor(Qt::white));
    nameItem.setFont(QFont("times",10));
    nameItem.setParentItem(this);
    nameItem.setPos(0,0);
    isactive=true;
    shoot=false;
    hasshield=false;
    hasice=false;
    hasspeedup=false;
    hasliquid=false;
    collideWithShield=false;
    isfast=false;
    isinvisible=false;
}

QRectF Enemy::boundingRect() const{
    return QRectF(-sx/2,-sy/2,sx,sy);
}

void Enemy::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->drawPixmap(-sx/2,-sy/2,sx,sy,pix,0,0,400,400);
}

QPainterPath Enemy::shape() const{
    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}

void Enemy::createRect(){
    //create the rects
    QPointF point0(scenePos().x()-rect_x,scenePos().y()-rect_y);
    rect_lu = new QRectF(point0,QSizeF(2*rect_x,2*rect_y));
}

void Enemy::judge(){
    QList<QGraphicsItem *> lu_list=game->items(game->mapFromScene(*rect_lu));
    calculate(lu_list);
}

void Enemy::increase(qreal d){
    qreal r=d/2;
    qreal R=sx/2;
    qreal delta_R=2*pow(r*r+R*R,0.5);
    sx=delta_R;
    sy=delta_R;
    weight=int(sx*200-4000);
    if(sx>40){
        shoot=true;
    }
}

void Enemy::decrease(qreal d){
    qreal r=d/2;
    qreal R=sx/2;
    qreal delta_R=2*pow(R*R-r*r,0.5);
    sx=delta_R;
    sy=delta_R;
    weight=int(sx*200-4000);
    if(sx<=40){
        shoot=false;
    }
}

void Enemy::reset(){
    sx=20.0;
    sy=20.0;
    v=20-0.025*sx;
    weight=0;
    setPos(qrand()%9000-4500,qrand()%5000-2500);
}

void Enemy::createbubble(qreal ang){
    bubble=new Bubble();
    game->scene->addItem(bubble);
    bubble->setRect(0,0,20,20);
    bubble->setBrush(QBrush(QColor(qrand()%256,qrand()%256,qrand()%256)));
    a=new QPropertyAnimation(bubble,"pos");
    a->setDuration(400);
    a->setKeyValueAt(0,QPointF(pos().x()+(sx)*qCos(ang)-10,pos().y()-(sy)*qSin(ang)-10));
    a->setKeyValueAt(1,QPointF(pos().x()+5*sx*qCos(ang)-10,pos().y()-5*sy*qSin(ang)-10));
    a->start();
    decrease(10);
}

void Enemy::move(){
    if(isactive){
        setZValue(sx/100);
        if(sx<=400)
            v=20-0.025*sx;
        else
            v=10;
        if(isfast==true){
            v*=2;
        }
        //judge the collisions
        QList<QGraphicsItem *> colliding_items = collidingItems();
        for (int i = 0, n = colliding_items.size(); i < n; ++i){
            if (typeid(*(colliding_items[i])) == typeid(QGraphicsEllipseItem)&&
                    colliding_items[i]->opacity()==1){
                // collided with food
                increase(5);
                if(colliding_items[i]->scene()!=NULL&&scene()!=NULL){
                    game->player->foodamount-=1;
                    delete colliding_items[i];
                }
            }
            else if (typeid(*(colliding_items[i])) == typeid(QGraphicsPixmapItem)&&colliding_items[i]->opacity()==1
                     &&typeid(*(colliding_items[i])) != typeid(Player)&&typeid(*(colliding_items[i])) != typeid(Enemy)){
                // collided with foodstar
                increase(8);
                if(colliding_items[i]->scene()!=NULL&&scene()!=NULL){
                    game->player->foodstaramount-=1;
                    delete colliding_items[i];
                }
            }
            else if(typeid(*(colliding_items[i])) == typeid(Cloud)&&
                    colliding_items[i]->scene()!=NULL&&scene()!=NULL){
                if(sx<75){
                    // collided with cloud
                    increase(30);
                    if(colliding_items[i]->scene()!=NULL&&scene()!=NULL){
                        game->cloud_amount-=1;
                        colliding_items[i]->setPos(qrand()%9000-4500,qrand()%5000-2500);
                    }
                }
                else{
                    createbubble((qrand()%629)/100);
                    createbubble((qrand()%629)/100);
                    createbubble((qrand()%629)/100);
                    createbubble((qrand()%629)/100);
                    direction=Angle(pos().x(),pos().y(),colliding_items[i]->pos().x(),colliding_items[i]->pos().y())+3.14;
                }
            }
            else if(typeid(*(colliding_items[i])) == typeid(Enemy)&&
                    colliding_items[i]->scene()!=NULL&&scene()!=NULL){
                // collided with another enemy
                Enemy *temp=dynamic_cast<Enemy *>(colliding_items[i]);
                col_id=temp->id;
                isCollided=true;
                game->assistance->collisions.append(this);
                game->assistance->enemyCollision();
            }
            else if(typeid(*(colliding_items[i])) == typeid(Bubble)&&
                    colliding_items[i]->scene()!=NULL&&scene()!=NULL){
                // increase the weight
                increase(10);
                if(colliding_items[i]->scene()!=NULL&&scene()!=NULL){
                    delete colliding_items[i];
                }
            }
            else if(collideWithShield==false&&typeid(*(colliding_items[i])) == typeid(Liquid)&&
                    colliding_items[i]->scene()!=NULL&&scene()!=NULL&&colliding_items[i]->parentItem()==NULL){
                if(shoot==true&&hasshield==false&&hasice==false&&hasliquid==false&&hasspeedup==false){
                    //collect the liquid
                    hasliquid=true;
                    colliding_items[i]->setParentItem(this);
                    colliding_items[i]->setPos(-15,-18.5);
                }
            }
            else if(collideWithShield==false&&typeid(*(colliding_items[i])) == typeid(Ice)&&
                    colliding_items[i]->scene()!=NULL&&scene()!=NULL&&colliding_items[i]->parentItem()==NULL){
                if(shoot==true&&hasshield==false&&hasice==false&&hasliquid==false&&hasspeedup==false){
                    //collect the ice
                    hasice=true;
                    colliding_items[i]->setParentItem(this);
                    colliding_items[i]->setPos(-15,-18.5);
                }
            }
            else if(collideWithShield==false&&typeid(*(colliding_items[i])) == typeid(Shield)&&
                    colliding_items[i]->scene()!=NULL&&scene()!=NULL&&colliding_items[i]->parentItem()==NULL){
                collideWithShield=true;
                if(shoot==true&&hasshield==false&&hasice==false&&hasliquid==false&&hasspeedup==false){
                    //collect the shield
                    hasshield=true;
                    colliding_items[i]->setParentItem(this);
                    colliding_items[i]->setPos(-15,-18.5);
                }
            }
            else if(collideWithShield==false&&typeid(*(colliding_items[i])) == typeid(Speedup)&&
                    colliding_items[i]->scene()!=NULL&&scene()!=NULL&&colliding_items[i]->parentItem()==NULL){
                if(shoot==true&&hasshield==false&&hasice==false&&hasliquid==false&&hasspeedup==false){
                    //collect the speedup
                    hasspeedup=true;
                    colliding_items[i]->setParentItem(this);
                    colliding_items[i]->setPos(-15,-18.5);
                }
            }
            else if(collideWithShield==false&&typeid(*(colliding_items[i])) == typeid(Blackhole)&&
                    colliding_items[i]->scene()!=NULL&&scene()!=NULL){
                if(shoot==true&&hasshield==false&&hasice==false&&hasliquid==false&&hasspeedup==false){
                    Blackhole *temp=dynamic_cast<Blackhole *>(colliding_items[i]);
                    int id_bind;
                    do{
                        id_bind=qrand()%6;
                    }while(id_bind==temp->id);
                    QPointF point=game->hole[id_bind]->pos();
                    setPos(point);
                    temp->setPos(qrand()%9000-4500,qrand()%5000-2500);
                    game->hole[id_bind]->setPos(qrand()%9000-4500,qrand()%5000-2500);
                }
            }
        }

        qreal dd=Distance(pos().x(),pos().y(),game->player->pos().x(),game->player->pos().y());
        qreal aa=Angle(pos().x(),pos().y(),game->player->pos().x(),game->player->pos().y());
        if(dd<=game->player->size_x/2+sx/2+sx&&sx>game->ratio*game->player->size_x){
            direction=aa;
            if(hasliquid){
                Liquid *temps=dynamic_cast<Liquid *>((childItems())[1]);
                temps->setParentItem(NULL);
                temps->setPixmap(QPixmap(":/images/energy.png"));
                ani=new QPropertyAnimation(temps,"pos");
                ani->setDuration(400);
                ani->setKeyValueAt(0,QPointF(pos().x()+sx*qCos(direction)-15,pos().y()-sy*qSin(direction)-18.5));
                ani->setKeyValueAt(1,QPointF(pos().x()+5*sx*qCos(direction)-15,pos().y()-5*sy*qSin(direction)-18.5));
                ani->start();
                QTimer *temp=new QTimer;
                QTimer *temp_det=new QTimer;
                QObject::connect(temp,SIGNAL(timeout()),temps,SLOT(deleteLater()));
                QObject::connect(temp,SIGNAL(timeout()),temp_det,SLOT(deleteLater()));
                QObject::connect(temp,SIGNAL(timeout()),temp,SLOT(deleteLater()));
                QObject::connect(temp_det,SIGNAL(timeout()),temps,SLOT(detect()));
                temp->start(400);
                temp_det->start(80);
                hasliquid=false;
                game->liquidamount-=1;
            }
            else if(hasice){
                Ice *temps=dynamic_cast<Ice *>((childItems())[1]);
                temps->setParentItem(NULL);
                temps->setPixmap(QPixmap(":/images/energy.png"));
                ani=new QPropertyAnimation(temps,"pos");
                ani->setDuration(400);
                ani->setKeyValueAt(0,QPointF(pos().x()+sx*qCos(direction)-15,pos().y()-sy*qSin(direction)-18.5));
                ani->setKeyValueAt(1,QPointF(pos().x()+5*sx*qCos(direction)-15,pos().y()-5*sy*qSin(direction)-18.5));
                ani->start();
                QTimer *temp=new QTimer;
                QTimer *temp_det=new QTimer;
                QObject::connect(temp,SIGNAL(timeout()),temps,SLOT(deleteLater()));
                QObject::connect(temp,SIGNAL(timeout()),temp_det,SLOT(deleteLater()));
                QObject::connect(temp,SIGNAL(timeout()),temp,SLOT(deleteLater()));
                QObject::connect(temp_det,SIGNAL(timeout()),temps,SLOT(detect()));
                temp->start(400);
                temp_det->start(80);
                hasice=false;
                game->iceamount-=1;
            }
            else if(hasspeedup){
                Speedup *temps=dynamic_cast<Speedup *>((childItems())[1]);
                delete temps;
                isfast=true;
                hasspeedup=false;
                QTimer *temp=new QTimer(this);
                QObject::connect(temp,SIGNAL(timeout()),this,SLOT(becomefast()));
                QObject::connect(temp,SIGNAL(timeout()),temp,SLOT(deleteLater()));
                temp->start(5000);
                game->speedupamount-=1;
            }
        }
        else if(dd<=game->player->size_x/2+sx/2+sx&&sx<game->ratio*game->player->size_x){
            direction=aa+3.14;
            if(hasshield){
                Shield *temps=dynamic_cast<Shield *>((childItems())[1]);
                if(temps->state==0){
                    hasshield=false;
                    temps->state=1;
                    temps->setOffset(15-sx/2,18.5-sy/2);
                    temps->setPixmap(QPixmap(":/images/gradient.png").scaled(QSize(sx,sy)));
                    ani=new QPropertyAnimation(temps,"opacity");
                    ani->setDuration(10000);
                    ani->setKeyValueAt(0,0);
                    ani->setKeyValueAt(0.125,1);
                    ani->setKeyValueAt(0.25,0);
                    ani->setKeyValueAt(0.375,1);
                    ani->setKeyValueAt(0.5,0);
                    ani->setKeyValueAt(0.625,1);
                    ani->setKeyValueAt(0.75,0);
                    ani->setKeyValueAt(0.875,1);
                    ani->setKeyValueAt(1,0);
                    ani->start();
                    isinvisible=true;
                    QTimer *temp=new QTimer(this);
                    QObject::connect(temp,SIGNAL(timeout()),temps,SLOT(endlife()));
                    QObject::connect(temp,SIGNAL(timeout()),this,SLOT(becomeInvisible()));
                    QObject::connect(temp,SIGNAL(timeout()),temp,SLOT(deleteLater()));
                    temp->start(10000);
                    game->shieldamount-=1;
                }
                else if(hasspeedup){
                    Speedup *temps=dynamic_cast<Speedup *>((childItems())[1]);
                    delete temps;
                    isfast=true;
                    hasspeedup=false;
                    QTimer *temp=new QTimer;
                    QObject::connect(temp,SIGNAL(timeout()),this,SLOT(becomefast()));
                    QObject::connect(temp,SIGNAL(timeout()),temp,SLOT(deleteLater()));
                    temp->start(5000);
                    game->speedupamount-=1;
                }
            }
        }

        if(pos().x()>=-4500&&pos().x()<=4500&&pos().y()>=-2500&&pos().y()<=2500)
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
}

void Enemy::updateRect(){
    judge();
    //update the rects
    rect_x=10*sx;
    rect_y=rect_x;
    QPointF point0(scenePos().x()-rect_x,scenePos().y()-rect_y);
    rect_lu->setRect(point0.x(),point0.y(),2*rect_x,2*rect_y);
}

void Enemy::becomeActive(){
    isactive=true;
}

void Enemy::becomefast(){
    isfast=false;
}

void Enemy::becomeInvisible(){
    isinvisible=false;
}

void Enemy::calculate(QList<QGraphicsItem *> &list){
    for(int i=0,n=list.size();i<n;i++){
        if(typeid(*(list[i]))==typeid(QGraphicsTextItem)||typeid(*(list[i]))==typeid(TimeCounter)
                ||typeid(list[i])==typeid(Weight)||list[i]->opacity()<1)
            continue;
        else if(typeid(*(list[i]))==typeid(Player)){
             temp=dynamic_cast<Player *>(list[i]);
             if(sx<temp->size_x){
                    direction=Angle(pos().x(),pos().y(),temp->pos().x(),temp->pos().y())+3.14;
                 }
             else if(sx>temp->size_x){
                 direction=Angle(pos().x(),pos().y(),temp->pos().x(),temp->pos().y());
             }
         }
         else if(typeid(*(list[i]))==typeid(Enemy)){
             tempp=dynamic_cast<Enemy *>(list[i]);
             if(tempp->id==id)
                 continue;
             if(sx<tempp->sx){
                 direction=Angle(pos().x(),pos().y(),tempp->pos().x(),tempp->pos().y())+3.14;
                 return;
             }
             else if(sx>tempp->sx){
                 direction=Angle(pos().x(),pos().y(),tempp->pos().x(),tempp->pos().y());
                 return;
             }
         }
         else if(typeid(*(list[i]))==typeid(Cloud)){
             tempc=dynamic_cast<Cloud *>(list[i]);
             if(sx<75){
                 direction=Angle(pos().x(),pos().y(),tempc->pos().x(),tempc->pos().y())+3.14;
                 return;
             }
             else{
                 direction=Angle(pos().x(),pos().y(),tempc->pos().x(),tempc->pos().y());
                 return;
             }
         }
         else if(typeid(*(list[i])) == typeid(QGraphicsPixmapItem)&&list[i]->opacity()==1
                 &&typeid(*(list[i])) != typeid(Player)&&typeid(*(list[i])) != typeid(Enemy)){
             direction=Angle(pos().x(),pos().y(),list[i]->pos().x(),list[i]->pos().y());
             return;
         }
        else{
            direction=Angle(pos().x(),pos().y(),list[i]->pos().x(),list[i]->pos().y());
            return;
        }
    }
    direction=(qrand()%629)/100;
    return;
}
