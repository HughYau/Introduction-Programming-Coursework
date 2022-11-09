#ifndef BOSS_H
#define BOSS_H
#include "enemy.h"

class Boss : public Enemy
{
    Q_OBJECT
public:
    Boss(WayPoint *startWayPoint, MainWindow *game);
    void enemyupgrade();
public slots:
    void shootWeapon();
};

#endif // BOSS_H
