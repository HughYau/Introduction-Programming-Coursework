#ifndef FIREBALL_H
#define FIREBALL_H


#include "bullet.h"


class Bullet4:public Bullet{
    Q_OBJECT
    Q_PROPERTY(QPoint m_currentPos READ currentPos WRITE setCurrentPos)
public:
    Bullet4(QPoint startPos, QPoint targetPoint, int damage, Enemy *target,MainWindow *game,QPixmap sprite);
    void hotenemy(Enemy *enemy);
public slots:
    void hitTarget();
};

#endif // FIREBALL_H
