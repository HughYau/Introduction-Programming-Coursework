#include "tower3.h"
#include "tower.h"
#include "enemy.h"
#include "bullet.h"
#include "mainwindow.h"
#include "utility.h"
#include "magic.h"
#include <QPainter>
#include <QColor>
#include <QTimer>
#include <QVector2D>
#include <QtMath>
#include<QDebug>

Tower3::Tower3(QPoint pos, MainWindow *game/*, const QPixmap &sprite*//* = QPixmap(":/image/tower.png"*/):Tower (pos,game)
{
    m_fireRateTimer = new QTimer(this);
    connect(m_fireRateTimer, SIGNAL(timeout()), this, SLOT(shootWeapon()));
    m_attackRange = 130;
    m_damage = 5;
    m_fireRate = 1500;
    m_sprite = QPixmap(":/image/wuqi1.png");
    upcost = 350;
    discost = 180;
    uplevel = 1;
    m_rotationSprite = 0.0;
    m_chooseEnemy = NULL;
}
void Tower3::shootWeapon()
{
    bullet = new Bullet3(m_pos, m_chooseEnemy->pos(), m_damage, m_chooseEnemy, m_game,QPixmap(":/image/bullet2.png"));
    bullet->m_speedrate =1 - 0.15*uplevel;
    bullet->move();
    if(existence)m_game->addBullet(bullet);
    else 	m_fireRateTimer->stop();
}
void Tower3::upgrade(){
    m_damage+=1;
    m_attackRange += 25;
     m_fireRate -=100;
     uplevel++;
     upcost +=200;
     discost += 0.6*upcost;
     if(uplevel == 2)m_sprite = QPixmap(":/image/wuqi2.png");
     else if(uplevel == 3)m_sprite = QPixmap(":/image/wuqi3.png");
}

