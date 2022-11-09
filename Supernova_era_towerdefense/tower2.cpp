#include "tower2.h"
#include "tower.h"
#include "enemy.h"
#include "bullet.h"
#include "mainwindow.h"
#include "utility.h"
#include "electric.h"
#include <QPainter>
#include <QColor>
#include <QTimer>
#include <QVector2D>
#include <QtMath>
#include<QDebug>

Tower2::Tower2(QPoint pos, MainWindow *game/*, const QPixmap &sprite*//* = QPixmap(":/image/tower.png"*/):Tower (pos,game)
{
    m_fireRateTimer = new QTimer(this);
    connect(m_fireRateTimer, SIGNAL(timeout()), this, SLOT(shootWeapon()));
    m_attackRange = 160;
    m_damage = 5;
    m_fireRate = 1500;
    m_sprite = QPixmap(":/image/10000.png");
    upcost = 350;
    discost = 180;
    uplevel = 1;
    m_rotationSprite = 0.0;
    m_chooseEnemy = NULL;
}
void Tower2::shootWeapon()
{
    bullet = new Bullet2(m_pos, m_chooseEnemy->pos(), m_damage, m_chooseEnemy, m_game,QPixmap(":/image/electric-energy.png"));
    bullet->m_per=0.05*uplevel;
    bullet->move();
    if(existence)m_game->addBullet(bullet);
    else 	m_fireRateTimer->stop();
}
void Tower2::upgrade(){
    m_damage+=2;
    m_attackRange += 25;
     m_fireRate -=100;
     uplevel++;
     upcost +=200;
     discost += 0.6*upcost;
     if(uplevel == 2)m_sprite = QPixmap(":/image/20000.png");
     else if(uplevel == 3)m_sprite = QPixmap(":/image/30000.png");
}
