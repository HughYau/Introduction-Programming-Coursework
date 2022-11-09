#include "tower.h"
#include "enemy.h"
#include "bullet.h"
#include "fireball.h"
#include "mainwindow.h"
#include "utility.h"
#include "res.h"
#include <QPainter>
#include <QColor>
#include <QTimer>
#include <QVector2D>
#include <QtMath>
#include<QDebug>

QSize Tower::ms_fixedSize(94, 94);
Tower::Tower(){}
Tower::Tower(QPoint pos, MainWindow *game)
	: m_attacking(false)
    , m_attackRange(140)
	, m_damage(10)
    , m_fireRate(1200)
    , upcost(200)
    , discost(120)
    , uplevel(1)
	, m_rotationSprite(0.0)
	, m_chooseEnemy(NULL)
	, m_game(game)
	, m_pos(pos)
{
	m_fireRateTimer = new QTimer(this);
	connect(m_fireRateTimer, SIGNAL(timeout()), this, SLOT(shootWeapon()));
    m_sprite = QPixmap(":/image/10111.png");
}

Tower::~Tower()
{
    delete m_fireRateTimer;
    m_fireRateTimer = NULL;
}

void Tower::checkEnemyInRange()
{
	if (m_chooseEnemy)
	{
        QVector2D normalized(m_chooseEnemy->pos() - m_pos);
        normalized.normalize();
        m_rotationSprite = qRadiansToDegrees(qAtan2(normalized.y(), normalized.x())) - 90;

		if (!collisionWithCircle(m_pos, m_attackRange, m_chooseEnemy->pos(), 1))
			lostSightOfEnemy();
	}
	else
	{
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

void Tower::draw(QPainter *painter) const
{
    QPen p;
    p.setStyle(Qt::DashDotLine);
    p.setWidth(4);
    p.setColor(Qt::green);
    painter->save();
    painter->setPen(p);
    painter->drawEllipse(m_pos, m_attackRange, m_attackRange);

    static const QPoint offsetPoint(-ms_fixedSize.width() / 2, -ms_fixedSize.height() / 2);
    painter->translate(m_pos);
    painter->rotate(m_rotationSprite);
    painter->drawPixmap(offsetPoint, m_sprite);
    painter->restore();
}

void Tower::attackEnemy()
{
	m_fireRateTimer->start(m_fireRate);
}

void Tower::chooseEnemyForAttack(Enemy *enemy)
{
	m_chooseEnemy = enemy;
	attackEnemy();
	m_chooseEnemy->getAttacked(this);
}

void Tower::shootWeapon()
{
    if(!fire)bullet = new Bullet(m_pos, m_chooseEnemy->pos(), m_damage, m_chooseEnemy, m_game,QPixmap(":/image/bulletorigin.png"));
    else bullet = new Bullet4(m_pos, m_chooseEnemy->pos(), m_damage, m_chooseEnemy, m_game,QPixmap(":/image/fireball.png"));
	bullet->move();
    if(existence){
        m_game->addBullet(bullet);
    }else{
        m_fireRateTimer->stop();
    }
}

void Tower::inExist()
{
    existence = false;
}

void Tower::targetKilled()
{
	if (m_chooseEnemy)
		m_chooseEnemy = NULL;

	m_fireRateTimer->stop();
}

void Tower::lostSightOfEnemy()
{
	m_chooseEnemy->gotLostSight(this);
	if (m_chooseEnemy)
		m_chooseEnemy = NULL;

	m_fireRateTimer->stop();
}
void Tower::upgrade(){
    m_damage+=3;
    m_attackRange += 20;
     m_fireRate -=100;
     uplevel++;
     upcost +=100;
     discost += 0.6*upcost;
     if(uplevel == 2)m_sprite = QPixmap(":/image/20111.png");
     else if(uplevel == 3)m_sprite = QPixmap(":/image/30111.png");
}
void Tower::uninstall(){
    existence = false;
}
bool Tower::contain(QPoint &pos)
{
    bool isXInHere = (m_pos.x() - ms_fixedSize.width()/2) < pos.x() && pos.x() < (m_pos.x() + ms_fixedSize.width()/2);
    bool isYInHere = (m_pos.y() - ms_fixedSize.height()/2) < pos.y() && pos.y() < (m_pos.y() + ms_fixedSize.height()/2);
    return isXInHere && isYInHere;
}


