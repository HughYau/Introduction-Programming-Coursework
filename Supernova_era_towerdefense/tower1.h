#ifndef TOWER1_H
#define TOWER1_H
#include "tower.h"


class Tower1:public Tower
{
     Q_OBJECT
public:
    Tower1(QPoint pos, MainWindow *game/*, const QPixmap &sprite*//* = QPixmap(":/image/tower.png"*/);
    void upgrade();
protected slots:
    void shootWeapon();
};

#endif // TOWER1_H
