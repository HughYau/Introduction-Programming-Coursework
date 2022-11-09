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



const QSize Enemy::ms_fixedSize(52, 52);

Enemy::Enemy(WayPoint *startWayPoint, MainWindow *game)
    : QObject(0)
    , m_active(false)
    ,chooseMainTower(false)
    , m_rotationSprite(0.0)
    , m_pos(startWayPoint->pos())
    , m_destinationWayPoint(startWayPoint->nextWayPoint())
    , m_game(game)
{
    up_sprite = QPixmap(":/image/enemy_up.png");
    down_sprite = QPixmap(":/image/enemy_down.png");
    left_sprite = QPixmap(":/image/enemy_left.png");
    right_sprite = QPixmap(":/image/enemy_right.png");
    m_maxHp = 50 ;
    m_currentHp = 50;
    m_walkingSpeed = 2.5;
    m_speed = 2.5;
    m_atk  =  2;
    m_rate =  500;
    m_range = 80;
    m_award = 200;
    m_stopped = false;
    m_slowed = false;
    m_hotted = false;
    hotms = new QTimer(this);
    connect(hotms,SIGNAL(timeout()),this,SLOT(hot()));
    fireRateTimer = new QTimer(this);
    connect(fireRateTimer,SIGNAL(timeout()),this,SLOT(shootWeapon()));
    slowms = new QTimer(this);
    connect(slowms,SIGNAL(timeout()),this,SLOT(slow()));
    enemyupgrade();
}

Enemy1::Enemy1(WayPoint *startWayPoint, MainWindow *game/*, const QPixmap &sprite*/)
    : Enemy(startWayPoint,game/*,sprite*/)
{
    up_sprite = QPixmap(":/image/enemy1_up.png");
    down_sprite = QPixmap(":/image/enemy1_down.png");
    left_sprite = QPixmap(":/image/enemy1_left.png");
    right_sprite = QPixmap(":/image/enemy1_right.png");
    m_maxHp = 40;
    m_currentHp = 40;
    m_walkingSpeed = 3;
    m_speed = 3;
    m_atk  =  3;
    m_rate =  800;
    m_range = 150;
    m_award = 200;
    m_stopped = false;
    m_slowed = false;
    fireRateTimer = new QTimer(this);
    connect(fireRateTimer,SIGNAL(timeout()),this,SLOT(shootWeapon()));

    enemyupgrade();
}

Enemy2::Enemy2(WayPoint *startWayPoint, MainWindow *game)
    : Enemy(startWayPoint,game/*,sprite*/)
{
    up_sprite = QPixmap(":/image/enemy2_up.png");
    down_sprite = QPixmap(":/image/enemy2_down.png");
    left_sprite = QPixmap(":/image/enemy2_left.png");
    right_sprite = QPixmap(":/image/enemy2_right.png");
    m_maxHp = 80;
    m_currentHp = 80;
    m_walkingSpeed = 2.0;
    m_speed = 2.0;
    m_atk  =  4;
    m_rate =  1000;
    m_range = 100;
    m_award = 400;
    m_stopped = false;
    m_slowed = false;
    fireRateTimer = new QTimer(this);
    connect(fireRateTimer,SIGNAL(timeout()),this,SLOT(shootWeapon()));

    enemyupgrade();

}
Enemy3::Enemy3(WayPoint *startWayPoint, MainWindow *game)
    : Enemy(startWayPoint,game)
{
    up_sprite = QPixmap(":/image/enemy3_up.png");
    down_sprite = QPixmap(":/image/enemy3_down.png");
    left_sprite = QPixmap(":/image/enemy3_left.png");
    right_sprite = QPixmap(":/image/enemy3_right.png");
    m_maxHp = 70;
    m_currentHp = 70;
    m_walkingSpeed = 2.0;
    m_speed = 2.0;
    m_atk  =  5;
    m_rate =  800;
    m_range = 200;
    m_award = 500;
    m_stopped = false;
    m_slowed = false;
    fireRateTimer = new QTimer(this);
    connect(fireRateTimer,SIGNAL(timeout()),this,SLOT(shootWeapon()));

    enemyupgrade();

}


Enemy::~Enemy()
{
    m_attackedTowersList.clear();
    m_destinationWayPoint = NULL;
    m_game = NULL;
    delete slowms;
    slowms = NULL;
    delete fireRateTimer;
    fireRateTimer = NULL;

}
void Enemy::enemyupgrade(){
    m_maxHp += 20*(stage-1);
    m_currentHp += 20*(stage-1);
    m_walkingSpeed += 0.2*(stage-1);
    m_rate -=  50*(stage-1);
    m_range += 20*(stage-1);
}
void Enemy::slow(){
    if(m_slowtime>=0)
        m_slowtime-=100;
    else{
        slowms->stop();
        m_walkingSpeed = m_speed;
        m_slowed = false;
    }

}
void Enemy::hot(){
    if(m_hottime>=0){
        m_hottime-=1000;
        m_currentHp = 0.95*m_currentHp;
}
    else{
        hotms->stop();
        m_hotted = false;
    }

}

void Enemy::doActivate()
{
    m_active = true;
}

void Enemy::move()
{
    if(!chooseMainTower){
        checkMainTower();
    }
    if (!m_active)
        return;
    if(m_stopped){
        return;
    }
    if (collisionWithCircle(m_pos, 1, m_destinationWayPoint->pos(), 1))
    {
        // 敌人抵达了一个航点
        if (m_destinationWayPoint->nextWayPoint())
        {
            // 还有下一个航点
            m_pos = m_destinationWayPoint->pos();
            m_destinationWayPoint = m_destinationWayPoint->nextWayPoint();
            if(!m_destinationWayPoint->nextWayPoint()) endposition = m_destinationWayPoint->pos();

        }

    }

    QPoint targetPoint = m_destinationWayPoint->pos();
    qreal movementSpeed = m_walkingSpeed;
    QVector2D normalized(targetPoint - m_pos);
    normalized.normalize();
    m_pos = m_pos + normalized.toPoint() * movementSpeed;
    if(normalized.toPoint() == QPoint(1,0)){//向右
        m_nowSprite = "right";
    }else if(normalized.toPoint() == QPoint(-1,0)){//向左
        m_nowSprite = "left";
    }else if(normalized.toPoint() == QPoint(0,-1)){//向上
        m_nowSprite = "up";
    }else if(normalized.toPoint() == QPoint(0,1)){//向下
        m_nowSprite = "down";
    }
}




void Enemy::checkMainTower()
{
    if(collisionWithCircle(pos(),m_range,endposition,1))
    {
        //qDebug() << "find it";
        m_stopped = true;
        chooseMainTower = true;
        fireRateTimer->start(m_rate);
    }
}

void Enemy::shootWeapon()
{
    bullet = new BulletToTower(m_pos,endposition, m_atk*m_waves, NULL, m_game,QPixmap(":/image/soldierbullet.png"));
    bullet->move();
    m_game->addBullet(bullet);
}

void Enemy::draw(QPainter *painter) const
{
    if (!m_active)
        return;

    painter->save();
    int Health_Bar_Width = m_maxHp/4 + 10;
    QPoint healthBarPoint = m_pos + QPoint(-Health_Bar_Width / 2 - 5, -ms_fixedSize.height() / 1.5);
    // 绘制血条
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::red);
    QRect healthBarBackRect(healthBarPoint, QSize(Health_Bar_Width, 4));
    painter->drawRect(healthBarBackRect);

    painter->setBrush(Qt::green);
    QRect healthBarRect(healthBarPoint, QSize((double)m_currentHp / m_maxHp * Health_Bar_Width, 4));
    painter->drawRect(healthBarRect);

    // 绘制偏转坐标,由中心+偏移=左上
    static const QPoint offsetPoint(-ms_fixedSize.width() / 2, -ms_fixedSize.height() / 2);
    painter->translate(m_pos);
    if(m_nowSprite == "up") painter->drawPixmap(offsetPoint, up_sprite);
    else if(m_nowSprite == "right") painter->drawPixmap(offsetPoint, right_sprite);
    else if(m_nowSprite == "left") painter->drawPixmap(offsetPoint, left_sprite);
    else if(m_nowSprite == "down") painter->drawPixmap(offsetPoint, down_sprite);
    if(m_slowed)painter->drawPixmap(offsetPoint, QPixmap(":/image/slow.png"));
    if(m_hotted)painter->drawPixmap(offsetPoint, QPixmap(":/image/fire.png"));
    painter->restore();
}

void Enemy::getRemoved()
{
    if (m_attackedTowersList.empty())
        return;

    foreach (Tower *attacker, m_attackedTowersList)
        attacker->targetKilled();
    // 通知game,此敌人已经阵亡
    m_game->removedEnemy(this);
}

void Enemy::getDamage(int damage)
{
    QMediaPlayer * player = new QMediaPlayer;
    player->setMedia(QUrl("qrc:/music/laser_shoot.wav"));
    player->setVolume(80);
    player->play();
    m_currentHp -= damage;

    // 阵亡,需要移除
    if (m_currentHp <= 0)
    {
        QMediaPlayer * player = new QMediaPlayer;
        player->setMedia(QUrl("qrc:/music/enemy_destroy.wav"));
        player->setVolume(80);
        player->play();
        m_game->awardGold(this->m_award);
        getRemoved();
    }
}

void Enemy::getAttacked(Tower *attacker)
{
    m_attackedTowersList.push_back(attacker);
}

// 表明敌人已经逃离了攻击范围
void Enemy::gotLostSight(Tower *attacker)
{
    m_attackedTowersList.removeOne(attacker);
}

QPoint Enemy::pos() const
{
    return m_pos;
}
void Enemy1::shootWeapon()
{
    bullet = new BulletToTower(m_pos,endposition, m_atk+1*m_waves, NULL, m_game,QPixmap(":/image/soldierbullet.png"));
    bullet->move();
    m_game->addBullet(bullet);
}
void Enemy2::shootWeapon()
{
    bullet = new BulletToTower(m_pos,endposition, m_atk+1*m_waves, NULL, m_game,QPixmap(":/image/soldierbullet1.png"));
    bullet->move();
    m_game->addBullet(bullet);
}
void Enemy3::shootWeapon()
{
    bullet = new BulletToTower(m_pos,endposition, m_atk+1*m_waves, NULL, m_game,QPixmap(":/image/bullet1.png"));
    bullet->move();
    m_game->addBullet(bullet);
}

