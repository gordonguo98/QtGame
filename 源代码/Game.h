#ifndef GAME_H
#define GAME_H

#include "Enemy.h"
#include "Cloud.h"
#include "Weight.h"
#include "Player.h"
#include "Myitem.h"
#include "Melee.h"
#include "Billboard.h"
#include "Assistance.h"
#include "Timecounter.h"
#include "Graphicsitem.h"
#include "Helper.h"

#include <QTimer>
#include <QWidget>
#include <QVector>
#include <QString>
#include <QKeyEvent>
#include <QTransform>
#include <QMediaPlayer>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QPropertyAnimation>
#include <QGraphicsPixmapItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsproxyWidget>

class Game:public QGraphicsView{
public:
    Game(QWidget * parent=0);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void menu();
    void rungame();
    void rungame2();
    void control();
    struct Names{
        QString name;
        bool notUsed;
    };
    //for sliding
    void initialize();
    void mixarray(QVector<int> &a);
    void countinverse(QVector<int> &a);
    void reset2();
    void finish();
    void menu2();
    void threeByThree();
    void fourByFour();
    void fiveByFive();
    void sixBySix();
    void slidingBlocks();
    bool isfinished();
    QGraphicsScene * scene;
    QGraphicsRectItem *item;
    QGraphicsEllipseItem *circle;
    QGraphicsPixmapItem *login;
    QGraphicsPixmapItem *menu_planet;
    QGraphicsPixmapItem *p_1;
    QGraphicsPixmapItem *p_n;
    QGraphicsPixmapItem *p_more;
    QGraphicsPixmapItem *gamemode;
    QGraphicsPixmapItem *yes;
    QGraphicsPixmapItem *p1_setting;
    QGraphicsPixmapItem *hundred;
    QGraphicsPixmapItem *ten;
    QGraphicsPixmapItem *one;
    QGraphicsPixmapItem *menuButton;
    QGraphicsPixmapItem *restartButton;
    QGraphicsPixmapItem *board;
    QGraphicsPixmapItem *home;
    QGraphicsPixmapItem *home2;
    QGraphicsPixmapItem *back;
    QGraphicsPixmapItem *menuboard;
    QGraphicsPixmapItem *three;
    QGraphicsPixmapItem *four;
    QGraphicsPixmapItem *five;
    QGraphicsPixmapItem *six;
    QGraphicsPixmapItem *title;
    QGraphicsPixmapItem *battleofbubbles;
    QGraphicsPixmapItem *slidingblocks;
    QGraphicsPixmapItem *introductionBoard;
    QGraphicsTextItem *modetext;
    QGraphicsTextItem *playeramount;//show the amount of players
    QGraphicsTextItem *timelimt;//show the time limited
    QGraphicsTextItem *best;
    QGraphicsTextItem *now;
    QVector<QString> numbers;
    QVector <Player *> childPlayer;
    QVector <Enemy *> enemy;
    QVector <Cloud *> clouds;
    QVector <Names> names;
    QVector <Blackhole *> hole;
    QVector <QTimer *> timer4;
    QVector <QTimer *> timer5;
    QVector<GraphicsItem *> blocks;
    QVector<QGraphicsPixmapItem *> buttons;
    QVector<int> a;
    QMediaPlayer *music;
    QMediaPlayer *menu_switch;
    QMediaPlayer *menu_click;
    QMediaPlayer *sliding;
    QMediaPlayer *finished;
    QMediaPlayer *switchup;
    QTimer * timer1;
    QTimer * timer2;
    QTimer * timer3;
    QTimer *timingcount;
    QTimer *timing;
    QTimer *timer;
    QPropertyAnimation *animation;
    QPropertyAnimation *ani;
    Weight * weight;
    Billboard * billboard;
    Helper *helper;
    Myitem *menu_item1;
    Myitem *menu_item2;
    Assistance *assistance;
    Player *player;
    TimeCounter *timecount;
    //items for melee mode
    int shieldamount;
    int liquidamount;
    int iceamount;
    int speedupamount;
    int blackholeamount;
    int players;//record the amount of childplayers
    int enemy_amount;
    int cloud_amount;
    int childPlayer_amount;
    int length;
    int width;
    int mode;//game mode
    int CLASSIC;
    int MELEE;
    int MAINMENU;
    int CHOOSINGMODE;
    int SETTING;
    int GAMING;
    int GAMEOVER;
    int GAMING2;
    int SLIDING;
    int CONTROL;
    int INTRODUCTION;
    int interface;//the current interface
    int pl_a;//playeramount
    int ti_l;//ttimelimt
    int lastmouse;
    int p1;
    int pn;
    int pmore;
    int pitem;
    int empty;
    int timedelay;
    int inverse;
    int MENU;
    int GAME3;
    int GAME4;
    int GAME5;
    int GAME6;
    int interface2;
    int FINISH;
    int gamenum;
    int bestRecord3;
    int bestRecord4;
    int bestRecord5;
    int bestRecord6;
    int lastStep;//3,4,5,6
    int lastkey;//0is up,1is down,2is left,3is right
    int time;
    qreal progressVal;
    qreal ratio;
    bool hasBegun;
    bool readIntroduction;
};

#endif // GAME_H
