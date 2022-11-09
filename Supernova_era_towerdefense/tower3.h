#ifndef TOWER3_H
#define TOWER3_H
#include "tower.h"


class Tower3:public Tower
{
     Q_OBJECT
public:
    Tower3(QPoint pos, MainWindow *game/*, const QPixmap &sprite*//* = QPixmap(":/image/tower.png"*/);
    void upgrade();
protected slots:
    void shootWeapon();
};


#endif // TOWER3_H
