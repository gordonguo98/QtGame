#include "Game.h"
#include "Cloud.h"
#include "Enemy.h"
#include "Player.h"
#include "Melee.h"
#include "Weight.h"
#include "Billboard.h"
#include "Timecounter.h"

#include <QTime>
#include <QList>
#include <QFont>
#include <QLine>
#include <QTimer>
#include <QBrush>
#include <QImage>
#include <QtMath>
#include <QDebug>
#include <QRectF>
#include <QWidget>
#include <QObject>
#include <QPointF>
#include <QVector>
#include <QThread>
#include <QPainter>
#include <QtGlobal>
#include <QComboBox>
#include <QResource>
#include <QTransform>
#include <QPushButton>
#include <QMouseEvent>
#include <QMediaPlayer>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QPropertyAnimation>
#include <QGraphicsProxyWidget>

extern Game * game;

qreal angle(qreal x1, qreal y1, qreal x2, qreal y2){
    qreal k=(y2-y1)/(x2-x1);
    if(x1<=x2&&y1<=y2)
        return 6.28-qAtan(k);
    else if(x1<=x2&&y1>=y2)
        return -qAtan(k);
    else
        return 3.14-qAtan(k);
}

void delay(qreal time){
    QTime last=QTime::currentTime();
    while(last.secsTo(QTime::currentTime())<=time);
}

Game::Game(QWidget *parent){
    length=1800;
    width=1000;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    //initialize the variables
    players=0;
    //menu_on=false;
    progressVal=0.0;
    ratio=1.1;
    enemy_amount=10;
    cloud_amount=20;
    childPlayer_amount=0;
    CLASSIC=0;
    MELEE=1;
    mode=CLASSIC;
    MAINMENU=0;
    CHOOSINGMODE=1;
    SETTING=2;
    GAMING=3;
    GAMEOVER=4;
    GAMING2=5;
    SLIDING=6;
    CONTROL=7;
    INTRODUCTION=8;
    interface=-1;
    pl_a=10;
    ti_l=3;
    p1=0;
    pn=1;
    pmore=2;
    pitem=3;
    lastmouse=-1;
    //set names list
    names.resize(30);
    names={{"Albert",1},{"Berlin",1},{"Cathy",1},{"Daniel",1},{"Sophia",1},{"Olivia",1},
           {"Emma",1},{"Isabella",1},{"Mia",1},{"Aria",1},{"Riley",1},{"Zoe",1},
           {"Amelia",1},{"Layla",1},{"Charlotte",1},{"Aubrey",1},{"Lily",1},{"Chloe",1},
           {"Harper",1},{"Evelyn",1},{"Emily",1},{"Scarlett",1},{"Camilla",1},{"Nora",1},
           {"Arianna",1},{"Kaylee",1},{"Madelyn",1},{"Isabelle",1},{"Sarah",1},{"Luna",1}};
    //initialize the clouds
    clouds.resize(cloud_amount);
    //initialize the childPlayers
    childPlayer.resize(childPlayer_amount);
    //initialize the objects
    assistance = new Assistance();
    circle = new QGraphicsEllipseItem ();
    circle->setBrush(QBrush(Qt::yellow));
    circle->setOpacity(0.3);
    //create the scene
    scene = new QGraphicsScene();
    scene->setSceneRect(-4500,-2500,9000,5000);
    scene->setBackgroundBrush(QBrush(Qt::black));
    setScene(scene);
    //set the login interface
    scene->addItem(assistance->pix0);
    scene->addItem(assistance->pix1);
    assistance->pix0->setPos(-900,-500);
    assistance->pix1->setPos(-900,-500);
    assistance->pix1->setVisible(false);

    //setWindowFlags(Qt::FramelessWindowHint);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setCacheMode(QGraphicsView::CacheBackground);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setFixedSize(length,width);
    QObject::connect(assistance->timer0,SIGNAL(timeout()),assistance,SLOT(enter()));
    assistance->timer0->start(15);

    show();
}

void Game::mousePressEvent(QMouseEvent *event){
    if(interface==CONTROL){
        if(itemAt(event->pos())==battleofbubbles)
            menu();
        else if(itemAt(event->pos())==slidingblocks)
            slidingBlocks();
    }
    else{
        if(interface==SLIDING){
            if(interface2==GAME3){
                if(itemAt(event->pos())==restartButton){
                    reset2();
                    threeByThree();
                }
                else if(itemAt(event->pos())==menuButton){
                    timing->stop();
                    menu2();
                }
            }
            else if(interface2==GAME4){
                if(itemAt(event->pos())==restartButton){
                    reset2();
                    fourByFour();
                }
                else if(itemAt(event->pos())==menuButton){
                    timing->stop();
                    menu2();
                }
            }
            if(interface2==GAME5){
                if(itemAt(event->pos())==restartButton){
                    reset2();
                    fiveByFive();
                }
                else if(itemAt(event->pos())==menuButton){
                    timing->stop();
                    menu2();
                }
            }
            if(interface2==GAME6){
                if(itemAt(event->pos())==restartButton){
                    reset2();
                    sixBySix();
                }
                else if(itemAt(event->pos())==menuButton){
                    timing->stop();
                    menu2();
                }
            }
            else if(interface2==FINISH){
                if(itemAt(event->pos())==home)
                    control();
                else if(itemAt(event->pos())==back){
                    delete board;
                    delete home;
                    delete back;
                    delete best;
                    delete now;
                    interface2=gamenum;
                }
            }
            else if(interface2==MENU){
                if(itemAt(event->pos())==three){
                    if(hasBegun==true)
                        reset2();
                    threeByThree();
                }
                if(itemAt(event->pos())==four){
                    if(hasBegun==true)
                        reset2();
                    fourByFour();
                }
                if(itemAt(event->pos())==five){
                    if(hasBegun==true)
                        reset2();
                    fiveByFive();
                }
                if(itemAt(event->pos())==six){
                    if(hasBegun==true)
                        reset2();
                    sixBySix();
                }
                if(itemAt(event->pos())==home2){
                    scene->clear();
                    control();
                }
            }
        }
        else{
            if(interface==INTRODUCTION){
                if(event->pos().x()>=1330&&event->pos().x()<=1388&&event->pos().y()>=262&&event->pos().y()<=319){
                    delete introductionBoard;
                    readIntroduction=true;
                    interface=MAINMENU;
                }
            }
            else if(interface==MAINMENU){
                if(((event->pos().x()-763)*(event->pos().x()-763)+(event->pos().y()-638)*(event->pos().y()-638)<6400)
                        &&((event->pos().x()-900)*(event->pos().x()-900)+(event->pos().y()-645)*(event->pos().y()-645)>8100)){
                    menu_click->setPosition(0.0);
                    menu_click->play();
                    //1p was pressed
                    p1_setting=new QGraphicsPixmapItem;
                    p1_setting->setPixmap(QPixmap(":/images/1psetting.png"));
                    scene->addItem(p1_setting);
                    p1_setting->setPos(-150,-50);
                    p1_setting->setZValue(1001);
                    //playeramount and timelimt
                    playeramount=new QGraphicsTextItem;
                    timelimt=new QGraphicsTextItem;
                    playeramount->setPlainText(QString::number(pl_a));
                    playeramount->setDefaultTextColor(Qt::white);
                    playeramount->setFont(QFont("times",12));
                    playeramount->setParentItem(p1_setting);
                    playeramount->setPos(175,10);
                    timelimt->setPlainText(QString::number(ti_l));
                    timelimt->setDefaultTextColor(Qt::white);
                    timelimt->setFont(QFont("times",12));
                    timelimt->setParentItem(p1_setting);
                    timelimt->setPos(175,45);
                    interface=SETTING;
                    return;
                }
                else if(((event->pos().x()-900)*(event->pos().x()-900)+(event->pos().y()-645)*(event->pos().y()-645)<8100)){
                    menu_click->setPosition(0.0);
                    menu_click->play();
                    //np was pressed
                    scene->clear();
                    if(mode==CLASSIC)
                        rungame();
                    else if(mode==MELEE)
                        rungame2();
                }
                else if(((event->pos().x()-1039)*(event->pos().x()-1039)+(event->pos().y()-638)*(event->pos().y()-638)<6400)
                        &&((event->pos().x()-900)*(event->pos().x()-900)+(event->pos().y()-645)*(event->pos().y()-645)>8100)){
                    //more was pressed
                    menu_click->setPosition(0.0);
                    menu_click->play();
                    control();
                }
                else if(event->pos().x()>878&&event->pos().x()<1083&&event->pos().y()>446&&event->pos().y()<480){
                    menu_click->setPosition(0.0);
                    menu_click->play();
                    //choose mode
                    interface=CHOOSINGMODE;
                    gamemode=new QGraphicsPixmapItem;
                    gamemode->setPixmap(QPixmap(":/images/gamemode.png"));
                    scene->addItem(gamemode);
                    gamemode->setPos(-150,-100);
                    gamemode->setZValue(1001);
                    return;
                }
            }
            else if(interface==CHOOSINGMODE){
                if(event->pos().x()>1010&&event->pos().x()<1025&&event->pos().y()>410&&event->pos().y()<425){
                    menu_click->setPosition(0.0);
                    menu_click->play();
                    //exit gamemodechoosing
                    delete gamemode;
                    yes->setPos(-1000,-1000);
                    interface=MAINMENU;
                }
                //if choose the clssic mode
                if(((event->pos().x()-790)*(event->pos().x()-790)+(event->pos().y()-472)*(event->pos().y()-472)<=625)){
                    menu_click->setPosition(0.0);
                    menu_click->play();
                    mode=CLASSIC;
                    modetext->setPlainText(QString("Classic"));
                    yes->setPos(-135,-54);
                }
                //if choose the paint mode
                if(((event->pos().x()-790)*(event->pos().x()-790)+(event->pos().y()-550)*(event->pos().y()-550)<=625)){
                    menu_click->setPosition(0.0);
                    menu_click->play();
                    mode=MELEE;
                    modetext->setPlainText(QString("Melee"));
                    yes->setPos(-135,26);
                }
            }
            else if(interface==SETTING){
                if(event->pos().x()>1021&&event->pos().x()<1036&&event->pos().y()>463&&event->pos().y()<478){
                    menu_click->setPosition(0.0);
                    menu_click->play();
                    //exit setting
                    delete p1_setting;
                    interface=MAINMENU;
                }
                //if add the playeramount
                if(event->pos().x()>964&&event->pos().x()<978&&event->pos().y()>474&&event->pos().y()<487){
                    menu_click->setPosition(0.0);
                    menu_click->play();
                    if(pl_a<20)
                        pl_a++;
                    playeramount->setPlainText(QString::number(pl_a));
                }
                //if subtract
                if(event->pos().x()>981&&event->pos().x()<996&&event->pos().y()>474&&event->pos().y()<487){
                    menu_click->setPosition(0.0);
                    menu_click->play();
                    if(pl_a>5)
                        pl_a--;
                    playeramount->setPlainText(QString::number(pl_a));
                }
                //if add the time
                if(event->pos().x()>964&&event->pos().x()<978&&event->pos().y()>509&&event->pos().y()<523){
                    menu_click->setPosition(0.0);
                    menu_click->play();
                    if(ti_l<30)
                        ti_l++;
                    timelimt->setPlainText(QString::number(ti_l));
                }
                //if subtract
                if(event->pos().x()>981&&event->pos().x()<996&&event->pos().y()>509&&event->pos().y()<523){
                    menu_click->setPosition(0.0);
                    menu_click->play();
                    if(ti_l>1)
                        ti_l--;
                    timelimt->setPlainText(QString::number(ti_l));
                }
            }
            else if(interface==GAMING||interface==GAMING2){
                //the point of mouse on the scene
                QPointF point = mapToScene(event->pos());
                //draw the cursor
                circle->setRect(QRectF(point.x()-30, point.y()-30, 60, 60));
                //this will change the scenepos of the mouse after
                scene->addItem(circle);
                //get the rotation
                if(player->scenePos().x()+player->size_x/2==point.x()&&
                        player->scenePos().y()+player->size_y/2==point.y())
                    ;
                else{
                    qreal rotate_angle=angle(player->scenePos().x(),
                                             player->scenePos().y(),point.x(),point.y());
                    player->setRotate(rotate_angle);
                }
            }
            else if(interface==GAMEOVER){
                if(event->pos().x()>717&&event->pos().x()<805&&event->pos().y()>710&&event->pos().y()<740){
                    menu_click->setPosition(0.0);
                    menu_click->play();
                    //home
                    scene->clear();
                    interface=MAINMENU;
                    menu();
                }

                if(event->pos().x()>856&&event->pos().x()<944&&event->pos().y()>710&&event->pos().y()<740){
                    menu_click->setPosition(0.0);
                    menu_click->play();
                    //restart
                    delete weight;
                    scene->clear();
                    if(mode==CLASSIC)
                        rungame();
                    else if(mode==MELEE)
                        rungame2();
                }

                if(event->pos().x()>994&&event->pos().x()<1083&&event->pos().y()>710&&event->pos().y()<740){
                    menu_click->setPosition(0.0);
                    menu_click->play();
                    //exit
                    scene->clear();
                    close();
                }
            }
        }
    }
}

void Game::mouseReleaseEvent(QMouseEvent *event){
    if(interface==CONTROL)
        return;
    if(interface==SLIDING)
        return;
    if(interface!=GAMING&&interface!=GAMING2)
        return;
    if(circle->scene()!=0)
        scene->removeItem(circle);
}

void Game::mouseMoveEvent(QMouseEvent *event){
    if(interface==CONTROL){
        if(dynamic_cast<QGraphicsPixmapItem *>(itemAt(event->pos()))==battleofbubbles)
            battleofbubbles->setScale(1.1);
        else
            battleofbubbles->setScale(1.0);
        if(dynamic_cast<QGraphicsPixmapItem *>(itemAt(event->pos()))==slidingblocks)
            slidingblocks->setScale(1.1);
        else
            slidingblocks->setScale(1.0);
    }
    else{
        if(interface==SLIDING){
            if(interface2==GAME3||interface2==GAME4||interface2==GAME5||interface2==GAME6){
                if(itemAt(event->pos())==menuButton)
                    menuButton->setPixmap(QPixmap(":/images/return_down.png"));
                else
                    menuButton->setPixmap(QPixmap(":/images/return_up.png"));
                if(itemAt(event->pos())==restartButton)
                    restartButton->setPixmap(QPixmap(":/images/restart_down.png"));
                else
                    restartButton->setPixmap(QPixmap(":/images/restart_up.png"));
                for(int i=0;i<4;i++){
                    if(itemAt(event->pos())==buttons[i])
                        buttons[i]->setOpacity(1.0);
                    else
                        buttons[i]->setOpacity(0.01);
                }
            }
            else if(interface2==FINISH){
                if(itemAt(event->pos())==home)
                    home->setOpacity(1.0);
                else
                    home->setOpacity(0.01);
                if(itemAt(event->pos())==back)
                    back->setOpacity(1.0);
                else
                    back->setOpacity(0.01);
            }
            else if(interface2==MENU){
                if(itemAt(event->pos())==three){
                    if(lastStep!=3){
                        switchup->setPosition(0.0);
                        switchup->play();
                    }
                    three->setScale(1.1);
                    lastStep=3;
                }
                else
                    three->setScale(1.0);
                if(itemAt(event->pos())==four){
                    if(lastStep!=4){
                        switchup->setPosition(0.0);
                        switchup->play();
                    }
                    four->setScale(1.1);
                    lastStep=4;
                }
                else
                    four->setScale(1.0);
                if(itemAt(event->pos())==five){
                    if(lastStep!=5){
                        switchup->setPosition(0.0);
                        switchup->play();
                    }
                    five->setScale(1.1);
                    lastStep=5;
                }
                else
                    five->setScale(1.0);
                if(itemAt(event->pos())==six){
                    if(lastStep!=6){
                        switchup->setPosition(0.0);
                        switchup->play();
                    }
                    six->setScale(1.1);
                    lastStep=6;
                }
                else
                    six->setScale(1.0);
                if(itemAt(event->pos())==home2){
                    if(lastStep!=7){
                        switchup->setPosition(0.0);
                        switchup->play();
                    }
                    home2->setOpacity(1.0);
                    lastStep=7;
                }
                else
                    home2->setOpacity(0.01);
            }
        }
        else{
            if(interface==MAINMENU){
                //change color when the mouse is on the menu buttons
                if(((event->pos().x()-763)*(event->pos().x()-763)+(event->pos().y()-638)*(event->pos().y()-638)<6400)
                        &&((event->pos().x()-900)*(event->pos().x()-900)+(event->pos().y()-645)*(event->pos().y()-645)>8100)){
                    if(lastmouse!=p1){
                        menu_switch->setPosition(0.0);
                        menu_switch->play();
                    }
                    //1p
                    p_1->setVisible(true);
                    lastmouse=p1;
                }
                else
                    p_1->setVisible(false);
                if(((event->pos().x()-900)*(event->pos().x()-900)+(event->pos().y()-645)*(event->pos().y()-645)<8100)){
                    if(lastmouse!=pn){
                        menu_switch->setPosition(0.0);
                        menu_switch->play();
                    }
                    //np
                    p_n->setVisible(true);
                    lastmouse=pn;
                }
                else
                    p_n->setVisible(false);
                if(((event->pos().x()-1039)*(event->pos().x()-1039)+(event->pos().y()-638)*(event->pos().y()-638)<6400)
                        &&((event->pos().x()-900)*(event->pos().x()-900)+(event->pos().y()-645)*(event->pos().y()-645)>8100)){
                    if(lastmouse!=pmore){
                        menu_switch->setPosition(0.0);
                        menu_switch->play();
                    }
                    //more
                    p_more->setVisible(true);
                    lastmouse=pmore;
                }
                else
                    p_more->setVisible(false);
                //rectitem on the menu
                if(event->pos().x()>878&&event->pos().x()<1083&&event->pos().y()>446&&event->pos().y()<480){
                    if(lastmouse!=pitem){
                        menu_switch->setPosition(0.0);
                        menu_switch->play();
                    }
                    item->setVisible(true);
                    lastmouse=pitem;
                }
                else
                    item->setVisible(false);
            }
        }
    }
}

void Game::wheelEvent(QWheelEvent *event){
    return;
}

void Game::keyPressEvent(QKeyEvent *event){
    if(interface==CONTROL)
        return;
    if(interface==SLIDING){
        if(interface2==GAME3){
            sliding->setPosition(0.0);
            sliding->play();
            if(lastkey!=1&&event->key()==Qt::Key_Up){
                buttons[0]->setOpacity(1.0);
                lastkey=0;
                if(empty>=7&&empty<=9)
                    return;
                else{
                    if(empty%3!=0&&itemAt(700+200*(empty%3-1),300+200*(empty/3)+200)!=NULL){
                        animation=new QPropertyAnimation(dynamic_cast<GraphicsItem *>(itemAt(700+200*(empty%3-1),300+200*(empty/3)+200)),"pos");
                        animation->setDuration(timedelay);
                        animation->setKeyValueAt(0,itemAt(700+200*(empty%3-1),300+200*(empty/3)+200)->pos());
                        animation->setKeyValueAt(1,QPointF(615+200*(empty%3-1),220+200*(empty/3)));
                        animation->start();
                        empty+=3;
                    }
                    else if(itemAt(700+2*200,300+200*(empty/3-1)+200)!=NULL){
                        animation=new QPropertyAnimation(dynamic_cast<GraphicsItem *>(itemAt(700+2*200,300+200*(empty/3-1)+200)),"pos");
                        animation->setDuration(timedelay);
                        animation->setKeyValueAt(0,itemAt(700+2*200,300+200*(empty/3-1)+200)->pos());
                        animation->setKeyValueAt(1,QPointF(615+2*200,220+200*(empty/3-1)));
                        animation->start();
                        empty+=3;
                    }
                }
            }
            else if(lastkey!=0&&event->key()==Qt::Key_Down){
                buttons[1]->setOpacity(1.0);
                lastkey=1;
                if(empty>=1&&empty<=3)
                    return;
                else{
                    if(empty%3!=0&&itemAt(700+200*(empty%3-1),300+200*(empty/3)-200)!=NULL){
                        animation=new QPropertyAnimation(dynamic_cast<GraphicsItem *>(itemAt(700+200*(empty%3-1),300+200*(empty/3)-200)),"pos");
                        animation->setDuration(timedelay);
                        animation->setKeyValueAt(0,itemAt(700+200*(empty%3-1),300+200*(empty/3)-200)->pos());
                        animation->setKeyValueAt(1,QPointF(615+200*(empty%3-1),220+200*(empty/3)));
                        animation->start();
                        empty-=3;
                    }
                    else if(itemAt(700+2*200,300+200*(empty/3-1)-200)!=NULL){
                        animation=new QPropertyAnimation(dynamic_cast<GraphicsItem *>(itemAt(700+2*200,300+200*(empty/3-1)-200)),"pos");
                        animation->setDuration(timedelay);
                        animation->setKeyValueAt(0,itemAt(700+2*220,300+200*(empty/3-1)-200)->pos());
                        animation->setKeyValueAt(1,QPointF(615+2*200,220+200*(empty/3-1)));
                        animation->start();
                        empty-=3;
                    }
                }
            }
            else if(lastkey!=3&&event->key()==Qt::Key_Left){
                buttons[2]->setOpacity(1.0);
                lastkey=2;
                if(empty%3==0)
                    return;
                else if(itemAt(700+200*(empty%3-1)+200,300+200*(empty/3))!=NULL){
                    animation=new QPropertyAnimation(dynamic_cast<GraphicsItem *>(itemAt(700+200*(empty%3-1)+200,300+200*(empty/3))),"pos");
                    animation->setDuration(timedelay);
                    animation->setKeyValueAt(0,itemAt(700+200*(empty%3-1)+200,300+200*(empty/3))->pos());
                    animation->setKeyValueAt(1,QPointF(615+200*(empty%3-1),220+200*(empty/3)));
                    animation->start();
                    empty+=1;
                }
            }
            else if(lastkey!=2&&event->key()==Qt::Key_Right){
                buttons[3]->setOpacity(1.0);
                lastkey=3;
                if(empty%3==1)
                    return;
                else{
                    if(empty%3!=0&&itemAt(700+200*(empty%3-1)-200,300+200*(empty/3))!=NULL){
                        animation=new QPropertyAnimation(dynamic_cast<GraphicsItem *>(itemAt(700+200*(empty%3-1)-200,300+200*(empty/3))),"pos");
                        animation->setDuration(timedelay);
                        animation->setKeyValueAt(0,itemAt(700+200*(empty%3-1)-200,300+200*(empty/3))->pos());
                        animation->setKeyValueAt(1,QPointF(615+200*(empty%3-1),220+200*(empty/3)));
                        animation->start();
                        empty-=1;
                    }
                    else if(itemAt(700+2*200-200,300+200*(empty/3-1))!=NULL){
                        animation=new QPropertyAnimation(dynamic_cast<GraphicsItem *>(itemAt(700+2*200-200,300+200*(empty/3-1))),"pos");
                        animation->setDuration(timedelay);
                        animation->setKeyValueAt(0,itemAt(700+2*200-200,300+200*(empty/3-1))->pos());
                        animation->setKeyValueAt(1,QPointF(615+2*200,220+200*(empty/3-1)));
                        animation->start();
                        empty-=1;
                    }
                }
            }
            timer=new QTimer;
            QObject::connect(timer,SIGNAL(timeout()),helper,SLOT(resetKey()));
            timer->start(timedelay);
        }
        else if(interface2==GAME4){
            sliding->setPosition(0.0);
            sliding->play();
            if(lastkey!=1&&event->key()==Qt::Key_Up){
                buttons[0]->setOpacity(1.0);
                lastkey=0;
                if(empty>=13&&empty<=16)
                    return;
                else{
                    if(empty%4!=0&&itemAt(675+150*(empty%4-1),275+150*(empty/4)+150)!=NULL){
                        animation=new QPropertyAnimation(dynamic_cast<GraphicsItem *>(itemAt(675+150*(empty%4-1),275+150*(empty/4)+150)),"pos");
                        animation->setDuration(timedelay);
                        animation->setKeyValueAt(0,itemAt(675+150*(empty%4-1),275+150*(empty/4)+150)->pos());
                        animation->setKeyValueAt(1,QPointF(607+150*(empty%4-1),215+150*(empty/4)));
                        animation->start();
                        empty+=4;
                    }
                    else if(itemAt(675+3*150,275+150*(empty/4-1)+150)!=NULL){
                        animation=new QPropertyAnimation(dynamic_cast<GraphicsItem *>(itemAt(675+3*150,275+150*(empty/4-1)+150)),"pos");
                        animation->setDuration(timedelay);
                        animation->setKeyValueAt(0,itemAt(675+3*150,275+150*(empty/4-1)+150)->pos());
                        animation->setKeyValueAt(1,QPointF(607+3*150,215+150*(empty/4-1)));
                        animation->start();
                        empty+=4;
                    }
                }
            }
            else if(lastkey!=0&&event->key()==Qt::Key_Down){
                buttons[1]->setOpacity(1.0);
                lastkey=1;
                if(empty>=1&&empty<=4)
                    return;
                else{
                    if(empty%4!=0&&itemAt(675+150*(empty%4-1),275+150*(empty/4)-150)!=NULL){
                        animation=new QPropertyAnimation(dynamic_cast<GraphicsItem *>(itemAt(675+150*(empty%4-1),275+150*(empty/4)-150)),"pos");
                        animation->setDuration(timedelay);
                        animation->setKeyValueAt(0,itemAt(675+150*(empty%4-1),275+150*(empty/4)-150)->pos());
                        animation->setKeyValueAt(1,QPointF(607+150*(empty%4-1),215+150*(empty/4)));
                        animation->start();
                        empty-=4;
                    }
                    else if(itemAt(675+3*150,275+150*(empty/4-1)-150)!=NULL){
                        animation=new QPropertyAnimation(dynamic_cast<GraphicsItem *>(itemAt(675+3*150,275+150*(empty/4-1)-150)),"pos");
                        animation->setDuration(timedelay);
                        animation->setKeyValueAt(0,itemAt(675+3*150,275+150*(empty/4-1)-150)->pos());
                        animation->setKeyValueAt(1,QPointF(607+3*150,215+150*(empty/4-1)));
                        animation->start();
                        empty-=4;
                    }
                }
            }
            else if(lastkey!=3&&event->key()==Qt::Key_Left){
                buttons[2]->setOpacity(1.0);
                lastkey=2;
                if(empty%4==0)
                    return;
                else if(itemAt(675+150*(empty%4-1)+150,275+150*(empty/4))!=NULL){
                    animation=new QPropertyAnimation(dynamic_cast<GraphicsItem *>(itemAt(675+150*(empty%4-1)+150,275+150*(empty/4))),"pos");
                    animation->setDuration(timedelay);
                    animation->setKeyValueAt(0,itemAt(675+150*(empty%4-1)+150,275+150*(empty/4))->pos());
                    animation->setKeyValueAt(1,QPointF(607+150*(empty%4-1),215+150*(empty/4)));
                    animation->start();
                    empty+=1;
                }
            }
            else if(lastkey!=2&&event->key()==Qt::Key_Right){
                buttons[3]->setOpacity(1.0);
                lastkey=3;
                if(empty%4==1)
                    return;
                else{
                    if(empty%4!=0&&itemAt(675+150*(empty%4-1)-150,275+150*(empty/4))!=NULL){
                        animation=new QPropertyAnimation(dynamic_cast<GraphicsItem *>(itemAt(675+150*(empty%4-1)-150,275+150*(empty/4))),"pos");
                        animation->setDuration(timedelay);
                        animation->setKeyValueAt(0,itemAt(675+150*(empty%4-1)-150,275+150*(empty/4))->pos());
                        animation->setKeyValueAt(1,QPointF(607+150*(empty%4-1),215+150*(empty/4)));
                        animation->start();
                        empty-=1;
                    }
                    else if(itemAt(675+3*150-150,275+150*(empty/4-1))!=NULL){
                        animation=new QPropertyAnimation(dynamic_cast<GraphicsItem *>(itemAt(675+3*150-150,275+150*(empty/4-1))),"pos");
                        animation->setDuration(timedelay);
                        animation->setKeyValueAt(0,itemAt(675+3*150-150,275+150*(empty/4-1))->pos());
                        animation->setKeyValueAt(1,QPointF(607+3*150,215+150*(empty/4-1)));
                        animation->start();
                        empty-=1;
                    }
                }
            }
            timer=new QTimer;
            QObject::connect(timer,SIGNAL(timeout()),helper,SLOT(resetKey()));
            timer->start(timedelay);
        }
        else if(interface2==GAME5){
            sliding->setPosition(0.0);
            sliding->play();
            if(lastkey!=1&&event->key()==Qt::Key_Up){
                buttons[0]->setOpacity(1.0);
                lastkey=0;
                if(empty>=21&&empty<=25)
                    return;
                else{
                    if(empty%5!=0&&itemAt(660+120*(empty%5-1),260+120*(empty/5)+120)!=NULL){
                        animation=new QPropertyAnimation(dynamic_cast<GraphicsItem *>(itemAt(660+120*(empty%5-1),260+120*(empty/5)+120)),"pos");
                        animation->setDuration(timedelay);
                        animation->setKeyValueAt(0,itemAt(660+120*(empty%5-1),260+120*(empty/5)+120)->pos());
                        animation->setKeyValueAt(1,QPointF(611+120*(empty%5-1),212+120*(empty/5)));
                        animation->start();
                        empty+=5;
                    }
                    else if(itemAt(660+4*120,260+120*(empty/5-1)+120)!=NULL){
                        animation=new QPropertyAnimation(dynamic_cast<GraphicsItem *>(itemAt(660+4*120,260+120*(empty/5-1)+120)),"pos");
                        animation->setDuration(timedelay);
                        animation->setKeyValueAt(0,itemAt(660+4*120,260+120*(empty/5-1)+120)->pos());
                        animation->setKeyValueAt(1,QPointF(611+4*120,212+120*(empty/5-1)));
                        animation->start();
                        empty+=5;
                    }
                }
            }
            else if(lastkey!=0&&event->key()==Qt::Key_Down){
                buttons[1]->setOpacity(1.0);
                lastkey=1;
                if(empty>=1&&empty<=5)
                    return;
                else{
                    if(empty%5!=0&&itemAt(660+120*(empty%5-1),260+120*(empty/5)-120)!=NULL){
                        animation=new QPropertyAnimation(dynamic_cast<GraphicsItem *>(itemAt(660+120*(empty%5-1),260+120*(empty/5)-120)),"pos");
                        animation->setDuration(timedelay);
                        animation->setKeyValueAt(0,itemAt(660+120*(empty%5-1),260+120*(empty/5)-120)->pos());
                        animation->setKeyValueAt(1,QPointF(611+120*(empty%5-1),212+120*(empty/5)));
                        animation->start();
                        empty-=5;
                    }
                    else if(itemAt(660+4*120,260+120*(empty/5-1)-120)!=NULL){
                        animation=new QPropertyAnimation(dynamic_cast<GraphicsItem *>(itemAt(660+4*120,260+120*(empty/5-1)-120)),"pos");
                        animation->setDuration(timedelay);
                        animation->setKeyValueAt(0,itemAt(660+4*120,260+120*(empty/5-1)-120)->pos());
                        animation->setKeyValueAt(1,QPointF(611+4*120,212+120*(empty/5-1)));
                        animation->start();
                        empty-=5;
                    }
                }
            }
            else if(lastkey!=3&&event->key()==Qt::Key_Left){
                buttons[2]->setOpacity(1.0);
                lastkey=2;
                if(empty%5==0)
                    return;
                else if(itemAt(660+120*(empty%5-1)+120,260+120*(empty/5))!=NULL){
                    animation=new QPropertyAnimation(dynamic_cast<GraphicsItem *>(itemAt(660+120*(empty%5-1)+120,260+120*(empty/5))),"pos");
                    animation->setDuration(timedelay);
                    animation->setKeyValueAt(0,itemAt(660+120*(empty%5-1)+120,260+120*(empty/5))->pos());
                    animation->setKeyValueAt(1,QPointF(611+120*(empty%5-1),211+120*(empty/5)));
                    animation->start();
                    empty+=1;
                }
            }
            else if(lastkey!=2&&event->key()==Qt::Key_Right){
                buttons[3]->setOpacity(1.0);
                lastkey=3;
                if(empty%5==1)
                    return;
                else{
                    if(empty%5!=0&&itemAt(660+120*(empty%5-1)-120,260+120*(empty/5))!=NULL){
                        animation=new QPropertyAnimation(dynamic_cast<GraphicsItem *>(itemAt(660+120*(empty%5-1)-120,260+120*(empty/5))),"pos");
                        animation->setDuration(timedelay);
                        animation->setKeyValueAt(0,itemAt(660+120*(empty%5-1)-120,260+120*(empty/5))->pos());
                        animation->setKeyValueAt(1,QPointF(611+120*(empty%5-1),212+120*(empty/5)));
                        animation->start();
                        empty-=1;
                    }
                    else if(itemAt(660+4*120-120,260+120*(empty/5-1))!=NULL){
                        animation=new QPropertyAnimation(dynamic_cast<GraphicsItem *>(itemAt(660+4*120-120,260+120*(empty/5-1))),"pos");
                        animation->setDuration(timedelay);
                        animation->setKeyValueAt(0,itemAt(660+4*120-120,260+120*(empty/5-1))->pos());
                        animation->setKeyValueAt(1,QPointF(611+4*120,212+120*(empty/5-1)));
                        animation->start();
                        empty-=1;
                    }
                }
            }
            timer=new QTimer;
            QObject::connect(timer,SIGNAL(timeout()),helper,SLOT(resetKey()));
            timer->start(timedelay);
        }
        else if(interface2==GAME6){
            sliding->setPosition(0.0);
            sliding->play();
            if(lastkey!=1&&event->key()==Qt::Key_Up){
                buttons[0]->setOpacity(1.0);
                lastkey=0;
                if(empty>=31&&empty<=36)
                    return;
                else{
                    if(empty%6!=0&&itemAt(650+100*(empty%6-1),250+100*(empty/6)+100)!=NULL){
                        animation=new QPropertyAnimation(dynamic_cast<GraphicsItem *>(itemAt(650+100*(empty%6-1),250+100*(empty/6)+100)),"pos");
                        animation->setDuration(timedelay);
                        animation->setKeyValueAt(0,itemAt(650+100*(empty%6-1),250+100*(empty/6)+100)->pos());
                        animation->setKeyValueAt(1,QPointF(607.5+100*(empty%6-1),210+100*(empty/6)));
                        animation->start();
                        empty+=6;
                    }
                    else if(itemAt(650+5*100,250+100*(empty/6-1)+100)!=NULL){
                        animation=new QPropertyAnimation(dynamic_cast<GraphicsItem *>(itemAt(650+5*100,250+100*(empty/6-1)+100)),"pos");
                        animation->setDuration(timedelay);
                        animation->setKeyValueAt(0,itemAt(650+5*100,250+100*(empty/6-1)+100)->pos());
                        animation->setKeyValueAt(1,QPointF(607.5+5*100,210+100*(empty/6-1)));
                        animation->start();
                        empty+=6;
                    }
                }
            }
            else if(lastkey!=0&&event->key()==Qt::Key_Down){
                buttons[1]->setOpacity(1.0);
                lastkey=1;
                if(empty>=1&&empty<=6)
                    return;
                else{
                    if(empty%6!=0&&itemAt(650+100*(empty%6-1),250+100*(empty/6)-100)!=NULL){
                        animation=new QPropertyAnimation(dynamic_cast<GraphicsItem *>(itemAt(650+100*(empty%6-1),250+100*(empty/6)-100)),"pos");
                        animation->setDuration(timedelay);
                        animation->setKeyValueAt(0,itemAt(650+100*(empty%6-1),250+100*(empty/6)-100)->pos());
                        animation->setKeyValueAt(1,QPointF(607.5+100*(empty%6-1),210+100*(empty/6)));
                        animation->start();
                        empty-=6;
                    }
                    else if(itemAt(650+5*100,250+100*(empty/6-1)-100)!=NULL){
                        animation=new QPropertyAnimation(dynamic_cast<GraphicsItem *>(itemAt(650+5*100,250+100*(empty/6-1)-100)),"pos");
                        animation->setDuration(timedelay);
                        animation->setKeyValueAt(0,itemAt(650+5*100,250+100*(empty/6-1)-100)->pos());
                        animation->setKeyValueAt(1,QPointF(607.5+5*100,210+100*(empty/6-1)));
                        animation->start();
                        empty-=6;
                    }
                }
            }
            else if(lastkey!=3&&event->key()==Qt::Key_Left){
                buttons[2]->setOpacity(1.0);
                lastkey=2;
                if(empty%6==0)
                    return;
                else if(itemAt(650+100*(empty%6-1)+100,250+100*(empty/6))!=NULL){
                    animation=new QPropertyAnimation(dynamic_cast<GraphicsItem *>(itemAt(650+100*(empty%6-1)+100,250+100*(empty/6))),"pos");
                    animation->setDuration(timedelay);
                    animation->setKeyValueAt(0,itemAt(650+100*(empty%6-1)+100,250+100*(empty/6))->pos());
                    animation->setKeyValueAt(1,QPointF(607.5+100*(empty%6-1),210+100*(empty/6)));
                    animation->start();
                    empty+=1;
                }
            }
            else if(lastkey!=2&&event->key()==Qt::Key_Right){
                buttons[3]->setOpacity(1.0);
                lastkey=3;
                if(empty%6==1)
                    return;
                else{
                    if(empty%6!=0&&itemAt(650+100*(empty%6-1)-100,250+100*(empty/6))!=NULL){
                        animation=new QPropertyAnimation(dynamic_cast<GraphicsItem *>(itemAt(650+100*(empty%6-1)-100,250+100*(empty/6))),"pos");
                        animation->setDuration(timedelay);
                        animation->setKeyValueAt(0,itemAt(650+100*(empty%6-1)-100,250+100*(empty/6))->pos());
                        animation->setKeyValueAt(1,QPointF(607.5+100*(empty%6-1),210+100*(empty/6)));
                        animation->start();
                        empty-=1;
                    }
                    else if(itemAt(650+5*100-100,250+100*(empty/6-1))!=NULL){
                        animation=new QPropertyAnimation(dynamic_cast<GraphicsItem *>(itemAt(650+5*100-100,250+100*(empty/6-1))),"pos");
                        animation->setDuration(timedelay);
                        animation->setKeyValueAt(0,itemAt(650+5*100-100,250+100*(empty/6-1))->pos());
                        animation->setKeyValueAt(1,QPointF(607.5+5*100,210+100*(empty/6-1)));
                        animation->start();
                        empty-=1;
                    }
                }
            }
            timer=new QTimer;
            QObject::connect(timer,SIGNAL(timeout()),helper,SLOT(resetKey()));
            timer->start(timedelay);
        }
        else if(interface2==FINISH){
            return;
        }
        else if(interface2==MENU){
            return;
        }
    }
    else{
        if(interface==GAMING||interface==GAMING2){
            if(player->shoot){
                if(event->key()==Qt::Key_Space){
                    if(interface==GAMING){
                        player->createBubble(player->rotate_angle);
                    }
                    else if(interface==GAMING2){
                        if(player->hasshield==false&&player->hasice==false&&player->hasliquid==false&&player->hasspeedup==false)
                            player->createBubble(player->rotate_angle);
                        else if(player->hasliquid==true){
                            Liquid *temps=dynamic_cast<Liquid *>((player->childItems())[1]);
                            temps->setParentItem(NULL);
                            temps->setPixmap(QPixmap(":/images/energy.png"));
                            ani=new QPropertyAnimation(temps,"pos");
                            ani->setDuration(400);
                            ani->setKeyValueAt(0,QPointF(player->pos().x()+(player->size_x)*qCos(player->rotate_angle)-15,player->pos().y()-(player->size_y)*qSin(player->rotate_angle)-18.5));
                            ani->setKeyValueAt(1,QPointF(player->pos().x()+5*player->size_x*qCos(player->rotate_angle)-15,player->pos().y()-5*player->size_y*qSin(player->rotate_angle)-18.5));
                            ani->start();
                            QTimer *temp=new QTimer;
                            QTimer *temp_det=new QTimer;
                            QObject::connect(temp,SIGNAL(timeout()),temps,SLOT(deleteLater()));
                            QObject::connect(temp,SIGNAL(timeout()),temp_det,SLOT(deleteLater()));
                            QObject::connect(temp,SIGNAL(timeout()),temp,SLOT(deleteLater()));
                            QObject::connect(temp_det,SIGNAL(timeout()),temps,SLOT(detect()));
                            temp->start(400);
                            temp_det->start(80);
                            player->hasliquid=false;
                            liquidamount-=1;
                        }
                        else if(player->hasice==true){
                            Ice *temps=dynamic_cast<Ice *>((player->childItems())[1]);
                            temps->setParentItem(NULL);
                            temps->setPixmap(QPixmap(":/images/energy.png"));
                            ani=new QPropertyAnimation(temps,"pos");
                            ani->setDuration(400);
                            ani->setKeyValueAt(0,QPointF(player->pos().x()+(player->size_x)*qCos(player->rotate_angle)-15,player->pos().y()-(player->size_y)*qSin(player->rotate_angle)-18.5));
                            ani->setKeyValueAt(1,QPointF(player->pos().x()+5*player->size_x*qCos(player->rotate_angle)-15,player->pos().y()-5*player->size_y*qSin(player->rotate_angle)-18.5));
                            ani->start();
                            QTimer *temp=new QTimer;
                            QTimer *temp_det=new QTimer;
                            QObject::connect(temp,SIGNAL(timeout()),temps,SLOT(deleteLater()));
                            QObject::connect(temp,SIGNAL(timeout()),temp_det,SLOT(deleteLater()));
                            QObject::connect(temp,SIGNAL(timeout()),temp,SLOT(deleteLater()));
                            QObject::connect(temp_det,SIGNAL(timeout()),temps,SLOT(detect()));
                            temp->start(400);
                            temp_det->start(80);
                            player->hasice=false;
                            iceamount-=1;
                        }
                        else if(player->hasshield==true){
                            Shield *temps=dynamic_cast<Shield *>((player->childItems())[1]);
                            if(temps->state==0){
                                player->hasshield=false;
                                temps->state=1;
                                temps->setOffset(15-player->size_x/2,18.5-player->size_y/2);
                                temps->setPixmap(QPixmap(":/images/gradient.png").scaled(QSize(player->size_x,player->size_y)));
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
                                player->isinvisible=true;
                                QTimer *temp=new QTimer;
                                QObject::connect(temp,SIGNAL(timeout()),temps,SLOT(endlife()));
                                QObject::connect(temp,SIGNAL(timeout()),player,SLOT(becomeInvisible()));
                                QObject::connect(temp,SIGNAL(timeout()),temp,SLOT(deleteLater()));
                                temp->start(10000);
                                shieldamount-=1;
                            }
                        }
                        else if(player->hasspeedup==true){
                            Speedup *temps=dynamic_cast<Speedup *>((player->childItems())[1]);
                            delete temps;
                            player->isfast=true;
                            player->hasspeedup=false;
                            QTimer *temp=new QTimer;
                            QObject::connect(temp,SIGNAL(timeout()),player,SLOT(becomeFast()));
                            QObject::connect(temp,SIGNAL(timeout()),temp,SLOT(deleteLater()));
                            temp->start(5000);
                            speedupamount-=1;
                        }
                    }
                }
            }
            if(event->key()==Qt::Key_Backspace){
                assistance->gameover();
                scene->clear();
                interface=MAINMENU;
                menu();
            }
        }
    }
}

void Game::keyReleaseEvent(QKeyEvent *event){
    if(interface==CONTROL)
        return;
    if(interface==SLIDING){
        if(interface2==GAME3){
            for(int i=0;i<4;i++)
                buttons[i]->setOpacity(0.01);
            if(isfinished()){
                timing->stop();
                finished->setPosition(0.0);
                finished->play();
                if(time<bestRecord3)
                    bestRecord3=time;
                finish();
            }
        }
        else if(interface2==GAME4){
            for(int i=0;i<4;i++)
                buttons[i]->setOpacity(0.01);
            if(isfinished()){
                timing->stop();
                finished->setPosition(0.0);
                finished->play();
                if(time<bestRecord4)
                    bestRecord4=time;
                finish();
            }
        }
        else if(interface2==GAME5){
            for(int i=0;i<4;i++)
                buttons[i]->setOpacity(0.01);
            if(isfinished()){
                timing->stop();
                finished->setPosition(0.0);
                finished->play();
                if(time<bestRecord5)
                    bestRecord5=time;
                finish();
            }
        }
        else if(interface2==GAME6){
            for(int i=0;i<4;i++)
                buttons[i]->setOpacity(0.01);
            if(isfinished()){
                timing->stop();
                finished->setPosition(0.0);
                finished->play();
                if(time<bestRecord6)
                    bestRecord6=time;
                finish();
            }
        }
        else if(interface2==FINISH){
            return;
        }
        else if(interface2==MENU){
            return;
        }
    }
}

void Game::menu(){
    scene->clear();
    scene->setSceneRect(-4500,-2500,9000,5000);
    scene->setBackgroundBrush(QBrush(Qt::black));
    centerOn(0,0);
    menu_switch=new QMediaPlayer;
    menu_click=new QMediaPlayer;
    menu_switch->setMedia(QUrl("qrc:/music/switch.mp3"));
    menu_click->setMedia(QUrl("qrc:/music/click.mp3"));
    //create menu items
    menu_planet=new QGraphicsPixmapItem();
    menu_planet->setPixmap(QPixmap(":/images/menu_planet.png"));
    scene->addItem(menu_planet);
    menu_planet->setPos(-250,-250);
    menu_planet->setZValue(2);
    menu_planet->setOpacity(0.5);
    //1p button
    p_1=new QGraphicsPixmapItem;
    p_1->setPixmap(QPixmap(":/images/1p.png"));
    scene->addItem(p_1);
    p_1->setPos(-250,-250);
    p_1->setZValue(1000);
    p_1->setVisible(false);
    //np button
    p_n=new QGraphicsPixmapItem;
    p_n->setPixmap(QPixmap(":/images/np.png"));
    scene->addItem(p_n);
    p_n->setPos(-250,-250);
    p_n->setZValue(1000);
    p_n->setVisible(false);
    //more button
    p_more=new QGraphicsPixmapItem;
    p_more->setPixmap(QPixmap(":/images/more.png"));
    scene->addItem(p_more);
    p_more->setPos(-250,-250);
    p_more->setZValue(1000);
    p_more->setVisible(false);

    item=new QGraphicsRectItem;
    scene->addItem(item);
    item->setRect(-22,-54,205,34);
    item->setBrush(QBrush(Qt::white));
    item->setOpacity(0.4);
    item->setZValue(1000);
    item->setVisible(false);

    yes=new QGraphicsPixmapItem;
    yes->setPixmap(QPixmap(":/images/yes.png"));
    scene->addItem(yes);
    yes->setZValue(1002);
    yes->setPos(-1000,-1000);

    //modetext
    modetext=new QGraphicsTextItem;
    modetext->setPlainText(QString("Classic"));
    modetext->setDefaultTextColor(Qt::white);
    modetext->setFont(QFont("times",12));
    scene->addItem(modetext);
    modetext->setPos(28,-54);
    modetext->setOpacity(0.7);
    modetext->setZValue(1000.5);

    //create the animation as background
    menu_item1=new Myitem();
    menu_item2=new Myitem();
    menu_item1->setPixmap(QPixmap(":/images/enemy0.png").scaled(QSize(200,200)));
    menu_item2->setPixmap(QPixmap(":/images/enemy3.png").scaled(QSize(150,150)));
    scene->addItem(menu_item1);
    scene->addItem(menu_item2);
    menu_item1->setPos(qrand()%length-length/2,qrand()%width-width/2);
    menu_item2->setPos(qrand()%length-length/2,qrand()%width-width/2);
    menu_item1->setZValue(0);
    menu_item2->setZValue(0);
    menu_item1->setOpacity(0.4);
    menu_item2->setOpacity(0.4);
    menu_item1->start();
    menu_item2->start();
    setMouseTracking(true);

    if(readIntroduction==false){
        introductionBoard=new QGraphicsPixmapItem;
        introductionBoard->setPixmap(QPixmap(":/images/introduction.png"));
        scene->addItem(introductionBoard);
        introductionBoard->setPos(-488,-238);
        introductionBoard->setZValue(5000);
        interface=INTRODUCTION;
    }
    else{
        interface=MAINMENU;
    }
}

void Game::rungame(){
    scene->setBackgroundBrush(QBrush(QColor(40,40,40)));
    //scale(0.2,0.2);
    interface=GAMING;
    enemy_amount=pl_a;
    timing=new QTimer();
    connect(timing,SIGNAL(timeout()),assistance,SLOT(gameover()));
    timing->start(60000*ti_l);
    timecount = new TimeCounter;
    scene->addItem(timecount);
    timecount->setZValue(20);
    timecount->setPos(mapToScene(890,10));
    timingcount=new QTimer;
    QObject::connect(timingcount,SIGNAL(timeout()),timecount,SLOT(showtime()));
    timingcount->start(1000);
    //scale(0.2,0.2);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    // create the player
    player = new Player();
    scene->addItem(player);
    player->setPos(0,0);
    player->setZValue(2);
    player->setFocus();
    player->setCacheMode(QGraphicsItem::ItemCoordinateCache);
    enemy.resize(enemy_amount);
    //create the enemy
    for(int i=0;i<enemy.size();i++){
        enemy[i]=new Enemy();
        scene->addItem(enemy[i]);
        enemy[i]->setPos(qrand()%9000-4500,qrand()%5000-2500);
        enemy[i]->createRect();
        enemy[i]->setCacheMode(QGraphicsItem::ItemCoordinateCache);
        enemy[i]->id=i;
        enemy[i]->col_id=-1;
        enemy[i]->isCollided=false;
    }
    // create the score
    weight = new Weight();
    scene->addItem(weight);
    weight->setZValue(100);
    weight->setPos(mapToScene(10,10));
    //set the billboard
    billboard = new Billboard();
    //initialize the timers
    timer1 = new QTimer();
    timer2 = new QTimer();
    timer3 = new QTimer();//opl2
    timer4.resize(enemy_amount);
    timer5.resize(cloud_amount);
    for(int i=0;i<enemy.size();i++)
        timer4[i]=new QTimer();
    for(int i=0;i<clouds.size();i++)
        timer5[i]=new QTimer();
    //move the player
    QObject::connect(timer1,SIGNAL(timeout()),player,SLOT(updateView()));//ok
    QObject::connect(timer1,SIGNAL(timeout()),player,SLOT(move()));
    QObject::connect(timer1,SIGNAL(timeout()),billboard,SLOT(updateBillboard()));//ok
    QObject::connect(timer2,SIGNAL(timeout()),player,SLOT(createfood()));//ok
    QObject::connect(timer3,SIGNAL(timeout()),player,SLOT(playmusic()));
    //create the clouds
    for(int i=0;i<clouds.size();i++){
        clouds[i]=new Cloud();
        scene->addItem(clouds[i]);
        clouds[i]->setPos(qrand()%9000-4500,qrand()%5000-2500);
        clouds[i]->setCacheMode(QGraphicsItem::ItemCoordinateCache);
        QObject::connect(timer5[i],SIGNAL(timeout()),clouds[i],SLOT(rotate()));//ok
        timer5[i]->start(80);
    }
    //move the enemies
    for(int i=0;i<enemy.size();i++){
        QObject::connect(timer4[i],SIGNAL(timeout()),enemy[i],SLOT(updateRect()));//ok
        QObject::connect(timer1,SIGNAL(timeout()),enemy[i],SLOT(move()));
        timer4[i]->start(300+50*(i-1));
    }
    timer1->start(80);
    timer2->start(100);
    timer3->start(40000);
    // set music
    music = new QMediaPlayer();
    music->setMedia(QUrl("qrc:/music/bg.mp3"));
    music->play();

    scene->update();
}

void Game::rungame2(){
    //scale(0.2,0.2);
    scene->setBackgroundBrush(QBrush(QColor(40,40,40)));
    interface=GAMING2;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    shieldamount=5;
    liquidamount=5;
    iceamount=5;
    speedupamount=5;
    blackholeamount=6;
    hole.resize(0);
    for(int i=0;i<shieldamount;i++){
        Shield *sld=new Shield;
        scene->addItem(sld);
        sld->setPos(qrand()%9000-4500,qrand()%5000-2500);
    }
    for(int i=0;i<liquidamount;i++){
        Liquid *lqd=new Liquid;
        scene->addItem(lqd);
        lqd->setPos(qrand()%9000-4500,qrand()%5000-2500);
    }
    for(int i=0;i<iceamount;i++){
        Ice *ice=new Ice;
        scene->addItem(ice);
        ice->setPos(qrand()%9000-4500,qrand()%5000-2500);
    }
    for(int i=0;i<speedupamount;i++){
        Speedup *spd=new Speedup;
        scene->addItem(spd);
        spd->setPos(qrand()%9000-4500,qrand()%5000-2500);
    }
    for(int i=0;i<blackholeamount;i++){
        Blackhole *blh=new Blackhole;
        scene->addItem(blh);
        blh->setPos(qrand()%9000-4500,qrand()%5000-2500);
        blh->id=i;
        hole.append(blh);
    }
    QTimer *supplement=new QTimer;
    QObject::connect(supplement,SIGNAL(timeout()),assistance,SLOT(createItems()));
    supplement->start(1000);
    //add others
    enemy_amount=pl_a;
    timing=new QTimer();
    connect(timing,SIGNAL(timeout()),assistance,SLOT(gameover()));
    timing->start(60000*ti_l);
    timecount = new TimeCounter;
    scene->addItem(timecount);
    timecount->setZValue(20);
    timecount->setPos(mapToScene(890,10));
    timingcount=new QTimer;
    QObject::connect(timingcount,SIGNAL(timeout()),timecount,SLOT(showtime()));
    timingcount->start(1000);
    // create the player
    player = new Player();
    scene->addItem(player);
    player->setPos(0,0);
    player->setZValue(2);
    player->setFocus();
    player->setCacheMode(QGraphicsItem::ItemCoordinateCache);
    enemy.resize(enemy_amount);
    //create the enemy
    for(int i=0;i<enemy.size();i++){
        enemy[i]=new Enemy();
        scene->addItem(enemy[i]);
        enemy[i]->setPos(qrand()%9000-4500,qrand()%5000-2500);
        enemy[i]->createRect();
        enemy[i]->setCacheMode(QGraphicsItem::ItemCoordinateCache);
        enemy[i]->id=i;
        enemy[i]->col_id=-1;
        enemy[i]->isCollided=false;
    }
    // create the score
    weight = new Weight();
    scene->addItem(weight);
    weight->setZValue(100);
    weight->setPos(mapToScene(10,10));
    //set the billboard
    billboard = new Billboard();
    //initialize the timers
    timer1 = new QTimer();
    timer2 = new QTimer();
    timer3 = new QTimer();//opl2
    timer4.resize(enemy_amount);
    timer5.resize(cloud_amount);
    for(int i=0;i<enemy.size();i++)
        timer4[i]=new QTimer();
    for(int i=0;i<clouds.size();i++)
        timer5[i]=new QTimer();
    //move the player
    QObject::connect(timer1,SIGNAL(timeout()),player,SLOT(updateView()));//ok
    QObject::connect(timer1,SIGNAL(timeout()),player,SLOT(move()));
    QObject::connect(timer1,SIGNAL(timeout()),billboard,SLOT(updateBillboard()));//ok
    QObject::connect(timer2,SIGNAL(timeout()),player,SLOT(createfood()));//ok
    QObject::connect(timer3,SIGNAL(timeout()),player,SLOT(playmusic()));
    //create the clouds
    for(int i=0;i<clouds.size();i++){
        clouds[i]=new Cloud();
        scene->addItem(clouds[i]);
        clouds[i]->setPos(qrand()%9000-4500,qrand()%5000-2500);
        clouds[i]->setCacheMode(QGraphicsItem::ItemCoordinateCache);
        QObject::connect(timer5[i],SIGNAL(timeout()),clouds[i],SLOT(rotate()));//ok
        timer5[i]->start(80);
    }
    //move the enemies
    for(int i=0;i<enemy.size();i++){
        QObject::connect(timer4[i],SIGNAL(timeout()),enemy[i],SLOT(updateRect()));//ok
        QObject::connect(timer1,SIGNAL(timeout()),enemy[i],SLOT(move()));
        timer4[i]->start(300+50*(i-1));
    }
    timer1->start(80);
    timer2->start(100);
    timer3->start(40000);
    // set music
    music = new QMediaPlayer();
    music->setMedia(QUrl("qrc:/music/bg.mp3"));
    music->play();
}

void Game::control(){
    scene->clear();
    scene->setSceneRect(-900,-500,1800,1000);
    scene->setBackgroundBrush(QBrush(Qt::black));
    interface=CONTROL;
    title=new QGraphicsPixmapItem;
    battleofbubbles=new QGraphicsPixmapItem;
    slidingblocks=new QGraphicsPixmapItem;
    title->setPixmap(QPixmap(":/images/Game2in1.png"));
    battleofbubbles->setPixmap(QPixmap(":/images/BattleOfBubbles.png"));
    slidingblocks->setPixmap(QPixmap(":/images/SlidingBlocks.png"));
    scene->addItem(title);
    scene->addItem(battleofbubbles);
    scene->addItem(slidingblocks);
    title->setPos(-310.5,-300);
    battleofbubbles->setPos(-200,-100);
    slidingblocks->setPos(-200,100);
    battleofbubbles->setTransformOriginPoint(200,45.5);
    slidingblocks->setTransformOriginPoint(200,45.5);
    setMouseTracking(true);
    show();
}

void Game::slidingBlocks(){
    scene->clear();
    interface=SLIDING;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    scene->setSceneRect(0,0,1800,1000);
    scene->setBackgroundBrush(QBrush(QPixmap(":/images/background.png")));
    setScene(scene);
    setFixedSize(1802,1002);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setMouseTracking(true);
    helper=new Helper;
    initialize();
    menu2();
    show();
}

void Game::initialize(){
    hasBegun=false;
    readIntroduction=false;
    MENU=1;
    FINISH=2;
    GAME3=3;
    GAME4=4;
    GAME5=5;
    GAME6=6;
    bestRecord3=1000;
    bestRecord4=1000;
    bestRecord5=1000;
    bestRecord6=1000;
    lastStep=-1;
    sliding=new QMediaPlayer;
    finished=new QMediaPlayer;
    switchup=new QMediaPlayer;
    sliding->setMedia(QUrl("qrc:/music/sliding.mp3"));
    finished->setMedia(QUrl("qrc:/music/finished.mp3"));
    switchup->setMedia(QUrl("qrc:/music/switch.mp3"));
    timedelay=100;
    menuButton=new QGraphicsPixmapItem;
    menuButton->setPixmap(QPixmap(":/images/return_up.png"));
    scene->addItem(menuButton);
    menuButton->setPos(50,50);
    restartButton=new QGraphicsPixmapItem;
    restartButton->setPixmap(QPixmap(":/images/restart_up.png"));
    scene->addItem(restartButton);
    restartButton->setPos(1650,50);
    hundred=new QGraphicsPixmapItem;
    hundred->setPixmap(QPixmap(":/images/000.png"));
    scene->addItem(hundred);
    hundred->setPos(50,347);
    ten=new QGraphicsPixmapItem;
    ten->setPixmap(QPixmap(":/images/000.png"));
    scene->addItem(ten);
    ten->setPos(200,347);
    one=new QGraphicsPixmapItem;
    one->setPixmap(QPixmap(":/images/000.png"));
    scene->addItem(one);
    one->setPos(350,347);
    buttons.resize(4);
    numbers.resize(10);
    for(int i=0;i<4;i++){
        buttons[i]=new QGraphicsPixmapItem;
        buttons[i]->setPixmap(QPixmap(QString(":/images/000")+QString::number(i)+QString(".png")));
        scene->addItem(buttons[i]);
        buttons[i]->setOpacity(0.01);
    }
    buttons[0]->setPos(1366.5,300);
    buttons[1]->setPos(1365,575);
    buttons[2]->setPos(1300,367.5);
    buttons[3]->setPos(1577,365.5);
    for(int i=0;i<10;i++){
        numbers[i]=QString(":/images/00")+QString::number(i)+QString(".png");
    }
    interface2=MENU;
}

bool Game::isfinished(){
    for(int i=1;i<blocks.size();i++){
        if(gamenum==GAME3){
            if(i%3!=0){
                if(dynamic_cast<GraphicsItem*>(itemAt(700+200*(i%3-1),300+200*(i/3)))!=blocks[i])
                    return false;
            }
            else{
                if(dynamic_cast<GraphicsItem*>(itemAt(700+2*200,300+200*(i/3-1)))!=blocks[i])
                    return false;
            }
        }
        else if(gamenum==GAME4){
            if(i%4!=0){
                if(dynamic_cast<GraphicsItem*>(itemAt(675+150*(i%4-1),275+150*(i/4)))!=blocks[i])
                    return false;
            }
            else{
                if(dynamic_cast<GraphicsItem*>(itemAt(675+3*150,275+150*(i/4-1)))!=blocks[i])
                    return false;
            }
        }
        else if(gamenum==GAME5){
            if(i%5!=0){
                if(dynamic_cast<GraphicsItem*>(itemAt(660+120*(i%5-1),260+120*(i/5)))!=blocks[i])
                    return false;
            }
            else{
                if(dynamic_cast<GraphicsItem*>(itemAt(660+4*120,260+120*(i/5-1)))!=blocks[i])
                    return false;
            }
        }
        else if(gamenum==GAME6){
            if(i%6!=0){
                if(dynamic_cast<GraphicsItem*>(itemAt(650+100*(i%6-1),250+100*(i/6)))!=blocks[i])
                    return false;
            }
            else{
                if(dynamic_cast<GraphicsItem*>(itemAt(650+5*100,250+100*(i/6-1)))!=blocks[i])
                    return false;
            }
        }
    }
    return true;
}

void Game::mixarray(QVector<int> &a){
    int first, second, temp;
    for(int i=1;i<a.size();i++){
        first=qrand()%(a.size()-1)+1;
        do{second=qrand()%(a.size()-1)+1;}while(second==first);
        temp=a[first];
        a[first]=a[second];
        a[second]=temp;
    }
}

void Game::countinverse(QVector<int> &a){
    for(int i=1;i<a.size()-1;i++){
        for(int j=i+1;j<a.size();j++){
            if(a[i]>a[j])
                inverse++;
        }
    }
}

void Game::reset2(){
    time=0;
    inverse=0;
    mixarray(a);
    countinverse(a);
    if(gamenum==GAME3){
        empty=qrand()%9+1;
    }
    if(gamenum==GAME4){
        if(inverse%2==0)
            empty=qrand()%4+5+(qrand()%2)*8;
        else
            empty=qrand()%4+1+(qrand()%2)*8;
    }
    if(gamenum==GAME5){
        empty=qrand()%25+1;
    }
    if(gamenum==GAME6){
        if(inverse%2==0)
            empty=qrand()%6+7+(qrand()%3)*12;
        else
            empty=qrand()%6+1+(qrand()%3)*12;
    }
    delete timing;
    lastkey=-1;
    hundred->setPixmap(QPixmap(":/images/000.png"));
    ten->setPixmap(QPixmap(":/images/000.png"));
    one->setPixmap(QPixmap(":/images/000.png"));
    for(int i=1;i<blocks.size();i++){
        delete blocks[i];
    }
}

void Game::finish(){
    board=new QGraphicsPixmapItem;
    home=new QGraphicsPixmapItem;
    back=new QGraphicsPixmapItem;
    best=new QGraphicsTextItem;
    now=new QGraphicsTextItem;
    board->setPixmap(QPixmap(":/images/finished.png"));
    home->setPixmap(QPixmap(":/images/home.png"));
    back->setPixmap(QPixmap(":/images/return.png"));
    best->setDefaultTextColor(Qt::yellow);
    best->setFont(QFont("Cooper Std",24));
    if(gamenum==GAME3)
        best->setPlainText(QString::number(bestRecord3)+QString(" s"));
    else if(gamenum==GAME4)
        best->setPlainText(QString::number(bestRecord4)+QString(" s"));
    else if(gamenum==GAME5)
        best->setPlainText(QString::number(bestRecord5)+QString(" s"));
    else if(gamenum==GAME6)
        best->setPlainText(QString::number(bestRecord6)+QString(" s"));
    now->setDefaultTextColor(Qt::yellow);
    now->setFont(QFont("Cooper Std",24));
    now->setPlainText(QString::number(time)+QString(" s"));
    scene->addItem(board);
    scene->addItem(home);
    scene->addItem(back);
    scene->addItem(best);
    scene->addItem(now);
    board->setPos(693,271);
    home->setPos(740,618);
    back->setPos(986,618);
    home->setOpacity(0.01);
    back->setOpacity(0.01);
    best->setPos(926,412);
    now->setPos(899,503);
    interface2=FINISH;
}

void Game::menu2(){
    menuboard=new QGraphicsPixmapItem;
    three=new QGraphicsPixmapItem;
    four=new QGraphicsPixmapItem;
    five=new QGraphicsPixmapItem;
    six=new QGraphicsPixmapItem;
    home2=new QGraphicsPixmapItem;
    menuboard->setPixmap(QPixmap(":/images/menu.png"));
    three->setPixmap(QPixmap(":/images/three.png"));
    four->setPixmap(QPixmap(":/images/four.png"));
    five->setPixmap(QPixmap(":/images/five.png"));
    six->setPixmap(QPixmap(":/images/six.png"));
    home2->setPixmap(QPixmap(":/images/home.png"));
    scene->addItem(menuboard);
    scene->addItem(three);
    scene->addItem(four);
    scene->addItem(five);
    scene->addItem(six);
    scene->addItem(home2);
    menuboard->setPos(590,92);
    three->setPos(690,242);
    four->setPos(690,392);
    five->setPos(690,542);
    six->setPos(690,692);
    home2->setPos(875,812);
    three->setTransformOriginPoint(210,55);
    four->setTransformOriginPoint(210,55);
    five->setTransformOriginPoint(210,55);
    six->setTransformOriginPoint(210,55);
    home2->setOpacity(0.01);
    interface2=MENU;
}

void Game::threeByThree(){
    if(interface2==MENU){
        delete menuboard;
        delete three;
        delete four;
        delete five;
        delete six;
        delete home2;
    }
    interface2=GAME3;
    gamenum=GAME3;
    hasBegun=true;
    time=0;
    inverse=0;
    lastkey=-1;
    a.resize(9);
    for(int i=0;i<9;i++)
        a[i]=i;
    mixarray(a);
    countinverse(a);
    empty=qrand()%9+1;
    timing=new QTimer;
    QObject::connect(timing,SIGNAL(timeout()),helper,SLOT(timing()));
    timing->start(1000);
    blocks.resize(9);
    for(int i=1;i<9;i++){
        blocks[a[i]]=new GraphicsItem;
        blocks[a[i]]->setPixmap(QPixmap(QString(":/images/")+QString::number(a[i])+QString(".png")).scaled(160,160));
        scene->addItem(blocks[a[i]]);
        if(i<empty){
            if(i%3!=0)
                blocks[a[i]]->setPos(615+200*(i%3-1),220+200*(i/3));
            else
                blocks[a[i]]->setPos(615+2*200,220+200*(i/3-1));
        }
        else{
            if((i+1)%3!=0)
                blocks[a[i]]->setPos(615+200*((i+1)%3-1),220+200*((i+1)/3));
            else
                blocks[a[i]]->setPos(615+2*200,220+200*((i+1)/3-1));
        }
    }
}

void Game::fourByFour(){
    if(interface2==MENU){
        delete menuboard;
        delete three;
        delete four;
        delete five;
        delete six;
        delete home2;
    }
    interface2=GAME4;
    gamenum=GAME4;
    hasBegun=true;
    time=0;
    inverse=0;
    lastkey=-1;
    a.resize(16);
    for(int i=0;i<16;i++)
        a[i]=i;
    mixarray(a);
    countinverse(a);
    if(inverse%2==0)
        empty=qrand()%4+5+(qrand()%2)*8;
    else
        empty=qrand()%4+1+(qrand()%2)*8;
    timing=new QTimer;
    QObject::connect(timing,SIGNAL(timeout()),helper,SLOT(timing()));
    timing->start(1000);
    blocks.resize(16);
    for(int i=1;i<16;i++){
        blocks[a[i]]=new GraphicsItem;
        blocks[a[i]]->setPixmap(QPixmap(QString(":/images/")+QString::number(a[i])+QString(".png")));
        scene->addItem(blocks[a[i]]);
        if(i<empty){
            if(i%4!=0)
                blocks[a[i]]->setPos(607+150*(i%4-1),215+150*(i/4));
            else
                blocks[a[i]]->setPos(607+3*150,215+150*(i/4-1));
        }
        else{
            if((i+1)%4!=0)
                blocks[a[i]]->setPos(607+150*((i+1)%4-1),215+150*((i+1)/4));
            else
                blocks[a[i]]->setPos(607+3*150,215+150*((i+1)/4-1));
        }
    }
}

void Game::fiveByFive(){
    if(interface2==MENU){
        delete menuboard;
        delete three;
        delete four;
        delete five;
        delete six;
        delete home2;
    }
    interface2=GAME5;
    gamenum=GAME5;
    hasBegun=true;
    time=0;
    inverse=0;
    lastkey=-1;
    a.resize(25);
    for(int i=0;i<25;i++)
        a[i]=i;
    mixarray(a);
    countinverse(a);
    empty=qrand()%25+1;
    timing=new QTimer;
    QObject::connect(timing,SIGNAL(timeout()),helper,SLOT(timing()));
    timing->start(1000);
    blocks.resize(25);
    for(int i=1;i<25;i++){
        blocks[a[i]]=new GraphicsItem;
        blocks[a[i]]->setPixmap(QPixmap(QString(":/images/")+QString::number(a[i])+QString(".png")).scaled(96,96));
        scene->addItem(blocks[a[i]]);
        if(i<empty){
            if(i%5!=0)
                blocks[a[i]]->setPos(611+120*(i%5-1),212+120*(i/5));
            else
                blocks[a[i]]->setPos(611+4*120,212+120*(i/5-1));
        }
        else{
            if((i+1)%5!=0)
                blocks[a[i]]->setPos(611+120*((i+1)%5-1),212+120*((i+1)/5));
            else
                blocks[a[i]]->setPos(611+4*120,212+120*((i+1)/5-1));
        }
    }
}

void Game::sixBySix(){
    if(interface2==MENU){
        delete menuboard;
        delete three;
        delete four;
        delete five;
        delete six;
        delete home2;
    }
    interface2=GAME6;
    gamenum=GAME6;
    hasBegun=true;
    time=0;
    inverse=0;
    lastkey=-1;
    a.resize(36);
    for(int i=0;i<36;i++)
        a[i]=i;
    mixarray(a);
    countinverse(a);
    if(inverse%2==0)
        empty=qrand()%6+7+(qrand()%3)*12;
    else
        empty=qrand()%6+1+(qrand()%3)*12;
    timing=new QTimer;
    QObject::connect(timing,SIGNAL(timeout()),helper,SLOT(timing()));
    timing->start(1000);
    blocks.resize(36);
    for(int i=1;i<36;i++){
        blocks[a[i]]=new GraphicsItem;
        blocks[a[i]]->setPixmap(QPixmap(QString(":/images/")+QString::number(a[i])+QString(".png")).scaled(80,80));
        scene->addItem(blocks[a[i]]);
        if(i<empty){
            if(i%6!=0)
                blocks[a[i]]->setPos(607.5+100*(i%6-1),210+100*(i/6));
            else
                blocks[a[i]]->setPos(607.5+5*100,210+100*(i/6-1));
        }
        else{
            if((i+1)%6!=0)
                blocks[a[i]]->setPos(607.5+100*((i+1)%6-1),210+100*((i+1)/6));
            else
                blocks[a[i]]->setPos(607.5+5*100,210+100*((i+1)/6-1));
        }
    }
}
