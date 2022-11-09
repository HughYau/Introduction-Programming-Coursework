#include "electric.h"
#include "bullet.h"
#include "enemy.h"
#include "mainwindow.h"
#include <QTimer>
#include <QPainter>
#include <QPropertyAnimation>
#include "utility.h"

Bullet2::Bullet2(QPoint startPos, QPoint targetPoint, int damage, Enemy *target,MainWindow *game,QPixmap sprite):Bullet(startPos,targetPoint,damage,target,game,sprite){

}
void Bullet2::hitTarget()
{
    if (m_game->m_enemyList.indexOf(m_target) != -1){
        m_target->getDamage(m_damage);
        peratk(m_target);
    }
    m_game->removedBullet(this);
}
void Bullet2::peratk(Enemy *enemy){
    enemy->m_currentHp-=(int)(enemy->m_currentHp*m_per);
}
