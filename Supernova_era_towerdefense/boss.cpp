#include "boss.h"
#include "enemy.h"
#include "res.h"
#include "waypoint.h"
#include "tower.h"
#include "utility.h"
#include "mainwindow.h"
#include "audioplayer.h"
#include <QPainter>
#include <QColor>
#include <QDebug>
#include <QMatrix>
#include <QVector2D>
#include <QtMath>
#include <QTimer>
#include <QDebug>
#include <QMediaPlayer>
#include "bullettotower.h"
Boss::Boss(WayPoint *startWayPoint, MainWindow *game/*, const QPixmap &sprite*/)
    : Enemy(startWayPoint,game/*,sprite*/)
{
    up_sprite = QPixmap(":/image/boss_up.png");
    down_sprite = QPixmap(":/image/boss_down.png");
    left_sprite = QPixmap(":/image/boss_left.png");
    right_sprite = QPixmap(":/image/boss_right.png");
    m_maxHp = 400;
    m_currentHp = 400;
    m_walkingSpeed = 1.5;
    m_speed = 1.5;

    m_atk  =  10;
    m_rate =  1200;
    m_range = 300;
    m_award = 1000;
    m_stopped = false;
    m_slowed = false;
    fireRateTimer = new QTimer(this);
    connect(fireRateTimer,SIGNAL(timeout()),this,SLOT(shootWeapon()));
    enemyupgrade();
}
void Boss::shootWeapon()
{
    bullet = new BulletToTower(m_pos,endposition, m_atk+1*m_waves, NULL, m_game,QPixmap(":/image/boss_bullet.png"));
    bullet->move();
    m_game->addBullet(bullet);
}
void Boss::enemyupgrade(){
    m_maxHp += 200*(stage-1);
    m_currentHp += 200*(stage-1);
    m_walkingSpeed += 0.3*(stage-1);
    m_rate -=  50*(stage-1);
    m_range += 20*(stage-1);

}
