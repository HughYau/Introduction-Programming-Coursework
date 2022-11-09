#-------------------------------------------------
#
# Project created by QtCreator 2013-08-22T22:46:33
#
#-------------------------------------------------

QT       += core gui multimedia

CONFIG += c++11
CONFIG +=resources_big

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TowerDefense
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    towerposition.cpp \
    tower.cpp \
    waypoint.cpp \
    enemy.cpp \
    bullet.cpp \
    icon.cpp \
    res.cpp \
    bomb.cpp \
    magic.cpp \
    electric.cpp \
    tower1.cpp \
    tower2.cpp \
    tower3.cpp \
    maintower.cpp \
    bullettotower.cpp \
    boss.cpp \
    fireball.cpp

HEADERS  += mainwindow.h \
    towerposition.h \
    tower.h \
    waypoint.h \
    utility.h \
    enemy.h \
    bullet.h \
    icon.h \
    res.h \
    bomb.h \
    magic.h \
    electric.h \
    tower1.h \
    tower2.h \
    tower3.h \
    maintower.h \
    bullettotower.h \
    boss.h \
    fireball.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc

DISTFILES +=
