#ifndef TOWER2_H
#define TOWER2_H
#include "tower.h"


class Tower2:public Tower
{
     Q_OBJECT
public:
    Tower2(QPoint pos, MainWindow *game/*, const QPixmap &sprite*//* = QPixmap(":/image/tower.png"*/);
    void upgrade();
protected slots:
    void shootWeapon();

};

#endif // TOWER2_H
