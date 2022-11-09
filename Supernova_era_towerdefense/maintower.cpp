#include "maintower.h"
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

MainTower::MainTower(QPoint pos, MainWindow *game) :Tower (pos,game)
{
    m_fireRateTimer = new QTimer(this);
    connect(m_fireRateTimer, SIGNAL(timeout()), this, SLOT(shootWeapon()));
    m_attackRange = 200;
    m_damage = 10;
    m_fireRate = 1200;
    m_sprite = QPixmap(":/image/temp_fire.png");
    upcost = 700;
    discost = 180;
    uplevel = 1;
    m_rotationSprite = 0.0;
    m_chooseEnemy = NULL;
}
void MainTower::draw(QPainter *painter) const{
    painter->save();
    painter->setPen(Qt::white);
    painter->drawEllipse(m_pos, m_attackRange, m_attackRange);

    painter->translate(m_pos);
    painter->restore();
    painter->drawPixmap(m_pos, m_sprite);

}
void MainTower::shootWeapon()
{
    QPixmap t;
    if(uplevel == 1)t = QPixmap(":/image/mainbullet1.png");
    else if(uplevel == 2)t = QPixmap(":/image/mainbullet3.png");
    else if(uplevel == 3)t =QPixmap(":/image/mainbullet2.png");
    bullet = new Bullet(m_pos, m_chooseEnemy->pos(), m_damage, m_chooseEnemy, m_game,t);
    bullet->move();
    if(existence)m_game->addBullet(bullet);
    else 	m_fireRateTimer->stop();
}
void MainTower::upgrade(){
    m_attackRange += 40;
     m_fireRate -=200;
     uplevel++;
     upcost +=300;
     discost += 60;
     if(uplevel == 2)m_sprite = QPixmap(":/image/tesla.png");
     else if(uplevel == 3)m_sprite = QPixmap(":/image/star_core.png");
}
void MainTower::checkEnemyInRange()
{
    if (m_chooseEnemy)
    {

        if (!collisionWithCircle(m_pos, m_attackRange, m_chooseEnemy->pos(), 1))
            lostSightOfEnemy();
    }
    else
    {
        // 遍历敌人,看是否有敌人在攻击范围内
        QList<Enemy *> enemyList = m_game->enemyList();
        foreach (Enemy *enemy, enemyList)
        {
            if (collisionWithCircle(m_pos, m_attackRange, enemy->pos(), 1))
            {
                chooseEnemyForAttack(enemy);
                break;

            }
        }
    }
}

