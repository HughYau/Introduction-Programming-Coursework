#include "magic.h"
#include "bullet.h"
#include "enemy.h"
#include "mainwindow.h"
#include <QTimer>
#include <QPainter>
#include <QPropertyAnimation>
#include "utility.h"
Bullet3::Bullet3(QPoint startPos, QPoint targetPoint, int damage, Enemy *target,MainWindow *game,QPixmap sprite):Bullet(startPos,targetPoint,damage,target,game,sprite){

}
void Bullet3::slowenemy(Enemy *enemy){

        enemy->m_slowtime = 2000;
        enemy->m_walkingSpeed =  min(enemy->m_speed*m_speedrate,enemy->m_walkingSpeed);
        enemy->m_slowed=true;
        enemy->slowms->start(100);
}
void Bullet3::hitTarget()
{

    if (m_game->m_enemyList.indexOf(m_target) != -1){
                slowenemy(m_target);
        m_target->getDamage(m_damage);
    }
    m_game->removedBullet(this);
}
