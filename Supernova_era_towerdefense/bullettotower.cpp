#include "bullettotower.h"
#include "bullet.h"
#include "enemy.h"
#include "mainwindow.h"
#include <QTimer>
#include <QPainter>
#include <QPropertyAnimation>
#include "utility.h"

BulletToTower::BulletToTower(QPoint startPos, QPoint targetPoint, int damage, Enemy *target,MainWindow *game,QPixmap sprite):Bullet(startPos,targetPoint,damage,target,game,sprite)
{

}


void BulletToTower::hitTarget()
{
    m_game->getHpDamage(m_damage);
    m_game->removedBullet(this);
}
