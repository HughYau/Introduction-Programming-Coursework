#include "tower1.h"
#include "tower.h"
#include "enemy.h"
#include "bullet.h"
#include "mainwindow.h"
#include "utility.h"
#include <QPainter>
#include <QColor>
#include <QTimer>
#include <QVector2D>
#include <QtMath>
#include<QDebug>
#include "bomb.h"

Tower1::Tower1(QPoint pos, MainWindow *game/*, const QPixmap &sprite*//* = QPixmap(":/image/tower.png"*/):Tower(pos,game)
{
    m_fireRateTimer = new QTimer(this);
    connect(m_fireRateTimer, SIGNAL(timeout()), this, SLOT(shootWeapon()));
    m_attackRange = 130;
    m_damage = 15;
    m_fireRate = 1600;
    m_sprite = QPixmap(":/image/10180.png");
    upcost = 300;
    discost = 150;
    uplevel = 1;
    m_rotationSprite = 0.0;
    m_chooseEnemy = NULL;
}
void Tower1::shootWeapon()
{
    bullet = new Bullet1(m_pos, m_chooseEnemy->pos(), m_damage, m_chooseEnemy, m_game,QPixmap(":/image/bomb.png"));
    bullet->m_ratk=5*uplevel;
    bullet->m_range=50+15*uplevel;
    bullet->move();
    if(existence)m_game->addBullet(bullet);
    else 	m_fireRateTimer->stop();
}
void Tower1::upgrade(){
    m_damage+=3;
    m_attackRange += 10;
     m_fireRate -=100;
     uplevel++;
     upcost +=100;
     discost += 0.6*upcost;
     if(uplevel == 2)m_sprite = QPixmap(":/image/20180.png");
     else if(uplevel == 3)m_sprite = QPixmap(":/image/30180.png");
}
