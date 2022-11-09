#ifndef TOWER_H
#define TOWER_H

#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <QObject>

class QPainter;
class Enemy;
class MainWindow;
class QTimer;
class Bullet;
class Tower:public QObject
{
    Q_OBJECT
public:
    Tower();
    Tower(QPoint pos, MainWindow *game);
    ~Tower();

    void draw(QPainter *painter) const;
    void targetKilled();
    void chooseEnemyForAttack(Enemy *enemy);
    void lostSightOfEnemy();
    bool contain(QPoint &pos);
    void uninstall();
    void inExist();

    virtual void upgrade();
    virtual void checkEnemyInRange();

    int				m_damage;
    int				m_fireRate;
    int             upcost;
    int             discost;
    int             uplevel;
    Bullet*         bullet;
    qreal			m_rotationSprite;

    static  QSize ms_fixedSize;
protected:
        void removeBullet();
        void damageEnemy();
        void attackEnemy();

    Enemy *			m_chooseEnemy;
    MainWindow *	m_game;
    QTimer *		m_fireRateTimer;
    QPoint          m_pos;
    QPixmap         m_sprite;
    bool            existence = true;
    bool			m_attacking;
    int				m_attackRange;


protected slots:
    void shootWeapon();

};

#endif // TOWER_H
