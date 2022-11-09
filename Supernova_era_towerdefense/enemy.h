
#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>
#include <QPoint>
#include <QSize>
#include <QPixmap>
#include<string>
#include"bullet.h"
using namespace std;

class WayPoint;
class QPainter;
class MainWindow;
class Tower;
class QTimer;

class Enemy : public QObject
{
    Q_OBJECT
public:
    Enemy(WayPoint *startWayPoint, MainWindow *game/*,const QPixmap &sprite = QPixmap(":/image/enemy_up.jpg")*/);
    ~Enemy();

    void draw(QPainter *painter) const;
    virtual void move();
    void getDamage(int damage);
    void getRemoved();
    void getAttacked(Tower *attacker);
    void gotLostSight(Tower *attacker);
    QPoint pos() const;
    void checkMainTower();
     double				m_currentHp;
     int                m_slowtime;
     int                m_hottime;
     QTimer*            slowms;
     QTimer*            hotms;
     bool               m_hotted;
     bool               m_slowed ;
     Bullet *           bullet;
     qreal			    m_walkingSpeed;
     qreal              m_speed;
public slots:
    void doActivate();
protected slots:
    void shootWeapon();
    void slow();
    void hot();

protected:
    virtual void enemyupgrade();
    QTimer *    fireRateTimer;
    bool			m_active;
    bool    m_stopped;
    bool chooseMainTower;
    qreal			m_rotationSprite;
    string  m_nowSprite;
    QPoint			m_pos;
    WayPoint *		m_destinationWayPoint;
    MainWindow *	m_game;
    QList<Tower *>	m_attackedTowersList;
    QPixmap up_sprite;
     QPixmap down_sprite;
    QPixmap left_sprite;
    QPixmap right_sprite;
    QPixmap	m_sprite;
    static const QSize ms_fixedSize;
    double				m_maxHp;
    int                m_atk;
    int                m_range;
    int                m_rate;
    int                m_award;
    QPoint             endposition;


};

class Enemy1 : public Enemy
{
    Q_OBJECT
public:
    Enemy1(WayPoint *startWayPoint, MainWindow *game);
public slots:
    void shootWeapon();
};

class Enemy2 : public Enemy
{
    Q_OBJECT
public:
    Enemy2(WayPoint *startWayPoint, MainWindow *game);
public slots:
    void shootWeapon();
};
class Enemy3 : public Enemy
{
    Q_OBJECT
public:
    Enemy3(WayPoint *startWayPoint, MainWindow *game);
public slots:
    void shootWeapon();
};





#endif // ENEMY_H
