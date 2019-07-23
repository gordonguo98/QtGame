#include "Player.h"
#include "Enemy.h"
#include "Game.h"
#include "Cloud.h"
#include "Melee.h"

#include <QList>
#include <QtMath>
#include <QRectF>
#include <QDebug>
#include <QtGlobal>
#include <QPainter>
#include <typeinfo>
#include <QGradient>
#include <QKeyEvent>
#include <QPainterPath>
#include <QMediaPlayer>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QPropertyAnimation>
#include <QGraphicsSceneMouseEvent>

extern Game * game;

qreal distance(qreal x1,qreal y1,qreal x2,qreal y2){
    return pow(pow(x1-x2,2)+pow(y1-y2,2),0.5);
}

Player::Player(QGraphicsItem *parent): QGraphicsPixmapItem(parent){
    v=20;
    rotate_angle=-1;
    last=-1;
    x0=0.0;
    y0=0.0;
    shoot=false;
    hasshield=false;
    hasice=false;
    hasliquid=false;
    hasspeedup=false;
    isinvisible=false;
    collideWithShield=false;
    isfast=false;
    isactive=true;
    size_x=20.0;
    size_y=20.0;
    foodamount=0;
    foodstaramount=0;
    playerWeight=0;
    pix.load(":/images/role.png");
    //setCacheMode(QGraphicsItem::ItemCoordinateCache);
    setVisible(true);
    setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsFocusable);
    //set the name
    int i=qrand()%30;
    name=game->names[i].name;
    game->names[i].notUsed=0;
    nameItem.setPlainText(name);
    nameItem.setDefaultTextColor(QColor(Qt::white));
    nameItem.setFont(QFont("times",10));
    nameItem.setParentItem(this);
    nameItem.setPos(0,0);
    //add two buttons for division and shooting
    divide=new QGraphicsPixmapItem(QPixmap(":/images/divide.png"));
    shooting=new QGraphicsPixmapItem(QPixmap(":/images/shoot.png"));
    game->scene->addItem(divide);
    game->scene->addItem(shooting);
    divide->setPos(500,300);
    shooting->setPos(700,300);
    divide->setOpacity(0.4);
    shooting->setOpacity(0.4);
    divide->setVisible(false);
    shooting->setVisible(false);
    divide->setZValue(20);
    shooting->setZValue(20);
    //load music
    eatfood=new QMediaPlayer;
    eatfoodstar=new QMediaPlayer;
    eatcloud=new QMediaPlayer;

    eatfood->setMedia(QUrl("qrc:/music/food.mp3"));
    eatfoodstar->setMedia(QUrl("qrc:/music/foodstar.mp3"));
    eatcloud->setMedia(QUrl("qrc:/music/cloud.mp3"));
}

QRectF Player::boundingRect() const{
    return QRectF(-size_x/2,-size_y/2,size_x,size_y);
}

void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->drawPixmap(-size_x/2,-size_y/2,size_x,size_y,pix,0,0,400,400);
}

QPainterPath Player::shape() const{
    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}

void Player::setRotate(qreal rotate){
    rotate_angle=rotate;
}

void Player::increase(qreal d){
    qreal r=d/2;
    qreal R=size_x/2;
    qreal delta_R=2*pow(r*r+R*R,0.5);
    size_x=delta_R;
    size_y=delta_R;
    playerWeight=int(size_x*200-4000);
    if(size_x>40){
        shoot=true;
        shooting->setVisible(true);
    }
}

void Player::decrease(qreal d){
    qreal r=d/2;
    qreal R=size_x/2;
    qreal delta_R=2*pow(R*R-r*r,0.5);
    size_x=delta_R;
    size_y=delta_R;
    playerWeight=int(size_x*200-4000);
    if(size_x<=40){
        shoot=false;
        shooting->setVisible(false);
    }
}

void Player::reset(){
    x0=0;
    y0=0;
    size_x=20.0;
    size_y=20.0;
    v=-0.025*size_x+20;
    playerWeight=0;
    game->scene->addItem(this);
    setPos(0,0);
    shoot=false;
    shooting->setVisible(false);
    isinvisible=true;
    QTimer *temp=new QTimer;
    QObject::connect(temp,SIGNAL(timeout()),this,SLOT(becomeInvisible()));
    temp->start(5000);
}

void Player::createBubble(qreal ang){
    bubble=new Bubble();
    game->scene->addItem(bubble);
    bubble->setRect(0,0,20,20);
    bubble->setBrush(QBrush(QColor(qrand()%256,qrand()%256,qrand()%256)));
    a=new QPropertyAnimation(bubble,"pos");
    a->setDuration(400);
    a->setKeyValueAt(0,QPointF(pos().x()+(size_x)*qCos(ang)-10,pos().y()-(size_y)*qSin(ang)-10));
    a->setKeyValueAt(1,QPointF(pos().x()+5*size_x*qCos(ang)-10,pos().y()-5*size_y*qSin(ang)-10));
    a->start();
    decrease(10);
}

void Player::move(){
    if(isactive){
        setZValue(20);
        //set the offset of the bounding
        offset_x=0;
        offset_y=0;
        //judge the collisions
        QList<QGraphicsItem *> colliding_items = collidingItems();
        for (int i = 0, n = colliding_items.size(); i < n; ++i){
            if (typeid(*(colliding_items[i])) == typeid(QGraphicsEllipseItem)&&
                    colliding_items[i]->opacity()==1&&typeid(*(colliding_items[i])) != typeid(Bubble)){
                eatfood->setPosition(0.0);
                eatfood->play();
                // increase the weight
                increase(5);
                game->weight->showWeight();
                if(colliding_items[i]->scene()!=NULL&&scene()!=NULL){
                    foodamount-=1;
                    delete colliding_items[i];
                }
            }
            else if (typeid(*(colliding_items[i])) == typeid(QGraphicsPixmapItem)
                     &&typeid(*(colliding_items[i])) != typeid(Enemy)&&colliding_items[i]->opacity()==1){
                eatfoodstar->setPosition(0.0);
                eatfoodstar->play();
                // increase the weight
                increase(8);
                game->weight->showWeight();
                if(colliding_items[i]->scene()!=NULL&&scene()!=NULL){
                    foodstaramount-=1;
                    delete colliding_items[i];
                }
            }
            else if(typeid(*(colliding_items[i])) == typeid(Enemy)&&
                    colliding_items[i]->scene()!=NULL&&scene()!=NULL){
                Enemy *temp=dynamic_cast<Enemy *>(colliding_items[i]);
                qreal d=distance(temp->scenePos().x(),temp->scenePos().y(),pos().x(),pos().y());
                if(size_x<=temp->sx&&isinvisible==false){
                    if(2*d<=temp->sx&&game->ratio*size_x<=temp->sx){
                        if((childItems()).size()>1){
                            if(typeid(*((childItems())[1]))==typeid(Shield)){
                                game->shieldamount-=1;
                                hasshield=false;
                                collideWithShield=false;
                            }
                            else if(typeid(*((childItems())[1]))==typeid(Ice)){
                                game->iceamount-=1;
                                hasice=false;
                            }
                            else if(typeid(*((childItems())[1]))==typeid(Speedup)){
                                game->speedupamount-=1;
                                hasspeedup=false;
                            }
                            else if(typeid(*((childItems())[1]))==typeid(Liquid)){
                                game->liquidamount-=1;
                                hasliquid=false;
                            }
                            delete ((childItems())[1]);
                        }
                        game->scene->removeItem(this);
                        reset();
                        temp->increase(size_x);
                        return;
                    }
                }
                else if(size_x>temp->sx){
                    if(2*d<=size_x&&game->ratio*temp->sx<=size_x){
                        increase(temp->sx);
                        temp->reset();
                        game->enemy_amount-=1;
                        game->weight->showWeight();
                    }
                }
            }
            else if(typeid(*(colliding_items[i])) == typeid(Cloud)&&
                    colliding_items[i]->scene()!=NULL&&scene()!=NULL){
                if(size_x<75){
                    eatcloud->setPosition(0.0);
                    eatcloud->play();
                    // increase the weight
                    increase(30);
                    game->weight->showWeight();
                    if(colliding_items[i]->scene()!=NULL&&scene()!=NULL){
                        game->cloud_amount-=1;
                        colliding_items[i]->setPos(qrand()%9000-4500,qrand()%5000-2500);
                    }
                }
                else{
                    createBubble((qrand()%629)/100);
                    createBubble((qrand()%629)/100);
                    createBubble((qrand()%629)/100);
                    createBubble((qrand()%629)/100);
                    game->weight->showWeight();
                }
            }
            else if(typeid(*(colliding_items[i])) == typeid(Bubble)&&
                    colliding_items[i]->scene()!=NULL&&scene()!=NULL){
                // increase the weight
                increase(10);
                game->weight->showWeight();
                if(colliding_items[i]->scene()!=NULL&&scene()!=NULL){
                    delete colliding_items[i];
                }
            }
            else if(collideWithShield==false&&typeid(*(colliding_items[i])) == typeid(Liquid)&&
                    colliding_items[i]->scene()!=NULL&&scene()!=NULL){
                if(shoot==true&&hasshield==false&&hasice==false&&hasliquid==false&&hasspeedup==false){
                    //collect the liquid
                    hasliquid=true;
                    colliding_items[i]->setParentItem(this);
                    colliding_items[i]->setPos(-15,-18.5);
                }
            }
            else if(collideWithShield==false&&typeid(*(colliding_items[i])) == typeid(Ice)&&
                    colliding_items[i]->scene()!=NULL&&scene()!=NULL){
                if(shoot==true&&hasshield==false&&hasice==false&&hasliquid==false&&hasspeedup==false){
                    //collect the ice
                    hasice=true;
                    colliding_items[i]->setParentItem(this);
                    colliding_items[i]->setPos(-15,-18.5);
                }
            }
            else if(collideWithShield==false&&typeid(*(colliding_items[i])) == typeid(Shield)&&
                    colliding_items[i]->scene()!=NULL&&scene()!=NULL){
                collideWithShield=true;
                if(shoot==true&&hasshield==false&&hasice==false&&hasliquid==false&&hasspeedup==false){
                    //collect the shield
                    hasshield=true;
                    colliding_items[i]->setParentItem(this);
                    colliding_items[i]->setPos(-15,-18.5);
                }
            }
            else if(collideWithShield==false&&typeid(*(colliding_items[i])) == typeid(Speedup)&&
                    colliding_items[i]->scene()!=NULL&&scene()!=NULL){
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
        //move the player
        if(rotate_angle==-1)
            return;
        //whether change direction
        if(rotate_angle!=last){
            if(size_x<=300)
                v=-0.025*size_x+20;
            else
                v=10;
            last=rotate_angle;
            setRotate(rotate_angle);
        }
        else if(v>0&&rotate_angle==last){
            if(size_x<=400)
                v=-0.025*size_x+20;
            else
                v=10;
        }

        if(isfast==true){
            v*=2;
        }

        //move the player
        if((1.57<=rotate_angle&&rotate_angle<=3.14&&pos().x()>-4500&&pos().y()>-2500)||
                (4.71<=rotate_angle&&rotate_angle<=6.28&&pos().x()<4500&&pos().y()<2500)||
                (0<=rotate_angle&&rotate_angle<1.57&&pos().x()<4500&&pos().y()>-2500)||
                (3.14<rotate_angle&&rotate_angle<4.71&&pos().x()>-4500&&pos().y()<2500)){
            //setPos(pos().x()+int(v*qCos(rotate_angle)),pos().y()-int(v*qSin(rotate_angle)));
            moveBy((v*qCos(rotate_angle)),-(v*qSin(rotate_angle)));
        }
        else if(pos().x()<=-4500&&-2500<pos().y()&&pos().y()<2500){
            if(1.57<=rotate_angle&&rotate_angle<=4.71){
                //setPos(pos().x(),pos().y()-int(v*qSin(rotate_angle)));
                moveBy(0,-(v*qSin(rotate_angle)));
            }
        }
        else if(-4500<pos().x()&&pos().x()<4500&&pos().y()<=-2500){
            if(0<=rotate_angle&&rotate_angle<=3.14){
                //setPos(pos().x()+int(v*qCos(rotate_angle)),pos().y());
                moveBy((v*qCos(rotate_angle)),0);
            }
        }
        else if(pos().x()>=4500&&-2500<pos().y()&&pos().y()<2500){
            if((0<=rotate_angle&&rotate_angle<=1.57)||
                    (4.71<=rotate_angle&&rotate_angle<=6.28)){
                //setPos(pos().x(),pos().y()-int(v*qSin(rotate_angle)));
                moveBy(0,-(v*qSin(rotate_angle)));
            }
        }
        else if(-4500<pos().x()&&pos().x()<4500&&pos().y()>=2500){
            if((3.14<=rotate_angle&&rotate_angle<=6.28)){
                //setPos(pos().x()+int(v*qCos(rotate_angle)),pos().y());
                moveBy((v*qCos(rotate_angle)),0);
            }
        }
    }
}

void Player::playmusic(){
    // play background music
    game->music->setPosition(0.00);
}

void Player::updateView(){
    //setOffset(-size_x/2,-size_y/2);
    game->centerOn(this);
    game->weight->setPos(game->mapToScene(10,10));
    game->timecount->setPos(game->mapToScene(890,10));
    divide->setPos(game->mapToScene(1400,800));
    shooting->setPos(game->mapToScene(1600,800));
    game->scene->update();
}

void Player::becomeInvisible(){
    isinvisible=false;
    collideWithShield=false;
}

void Player::becomeFast(){
    isfast=false;
}

void Player::becomeActive(){
    isactive=true;
}

void Player::createfood(){
    if(foodamount>=1000)
        ;
    else{
        for(int i=0;i<100;i++){
            qreal radias=4.0;
            qreal x=qrand()%9000-4500;
            qreal y=qrand()%5000-2500;
            QRectF rect(x, y, 2*radias, 2*radias);
            food=new QGraphicsEllipseItem();
            game->scene->addItem(food);
            QRadialGradient r(QPointF(x,y),radias,QPointF(x-0.5,y+0.5));
            r.setColorAt(0,QColor(255,255,255,255));
            r.setColorAt(1,QColor(qrand()%256,qrand()%256,qrand()%256,255));
            food->setBrush(r);
            food->setRect(rect);
            food->setCacheMode(QGraphicsItem::ItemCoordinateCache);
        }
        foodamount+=100;
    }
    if(foodstaramount>=200)
        ;
    else{
        for(int i=0;i<20;i++){
            qreal radias=30;
            foodstar=new QGraphicsPixmapItem();
            game->scene->addItem(foodstar);
            foodstar->setPixmap(QPixmap(":/images/food.png").scaled(radias,radias));
            foodstar->setPos(qrand()%9000-4500, qrand()%5000-2500);
            foodstar->setCacheMode(QGraphicsItem::ItemCoordinateCache);
        }
        foodstaramount+=20;
    }
}
