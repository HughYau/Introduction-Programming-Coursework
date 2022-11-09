#ifndef MAINTOWER_H
#define MAINTOWER_H
#include "tower.h"


class MainTower : public Tower
{
    Q_OBJECT
public:
    MainTower(QPoint pos , MainWindow *game);
    void draw(QPainter *painter) const;
    void upgrade();
    void checkEnemyInRange();
protected slots:
    void shootWeapon();
};

#endif // MAINTOWER_H
