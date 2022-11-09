#include "bomb.h"
#include "bullet.h"
#include "enemy.h"
#include "mainwindow.h"
#include <QTimer>
#include <QPainter>
#include <QPropertyAnimation>
#include "utility.h"

Bullet1::Bullet1(QPoint startPos, QPoint targetPoint, int damage, Enemy *target,MainWindow *game,QPixmap sprite):Bullet(startPos,targetPoint,damage,target,game,sprite){
}
void Bullet1::rangeatk()
{
    foreach(Enemy *enemy,m_game->m_enemyList){
        if (collisionWithCircle(m_currentPos,m_range, enemy->pos(), 10)&&enemy!=m_target)
            enemy->getDamage(m_ratk);
    }
}
void Bullet1::hitTarget()
{

    if (m_game->enemyList().indexOf(m_target) != -1){
        m_target->getDamage(m_damage);
        rangeatk();
    }
    m_game->removedBullet(this);
}
