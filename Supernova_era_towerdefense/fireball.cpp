#include "fireball.h"
#include "bullet.h"
#include "enemy.h"
#include "mainwindow.h"
#include <QTimer>
#include <QPainter>
#include <QPropertyAnimation>
#include "utility.h"
Bullet4::Bullet4(QPoint startPos, QPoint targetPoint, int damage, Enemy *target,MainWindow *game,QPixmap sprite):Bullet(startPos,targetPoint,damage,target,game,sprite){

}
void Bullet4::hotenemy(Enemy *enemy){

        enemy->m_hottime = 3000;
        enemy->m_hotted=true;
        enemy->hotms->start(1000);
}
void Bullet4::hitTarget()
{

    if (m_game->m_enemyList.indexOf(m_target) != -1){
                hotenemy(m_target);
        m_target->getDamage(m_damage);
    }
    m_game->removedBullet(this);
}
