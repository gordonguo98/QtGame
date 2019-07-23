#-------------------------------------------------
#
# Project created by QtCreator 2018-07-22T19:51:56
#
#-------------------------------------------------

QT       += core gui widgets\
         multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Game2in1
TEMPLATE = app

QMAKE_CXXFLAGS +=  -Wno-unused-parameter

RC_FILE = battleball.rc

CONFIG += resources_big

SOURCES += main.cpp \
    Enemy.cpp \
    Game.cpp \
    Player.cpp \
    Billboard.cpp \
    Weight.cpp \
    Assistance.cpp \
    Cloud.cpp \
    Myitem.cpp \
    Bubble.cpp \
    Timecounter.cpp \
    Graphicsitem.cpp \
    Helper.cpp

HEADERS  += \
    Enemy.h \
    Game.h \
    Player.h \
    Billboard.h \
    Weight.h \
    Assistance.h \
    Cloud.h \
    Myitem.h \
    Bubble.h \
    Timecounter.h \
    Melee.h \
    Graphicsitem.h \
    Helper.h

RESOURCES += \
    res.qrc
