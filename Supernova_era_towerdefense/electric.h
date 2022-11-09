#ifndef ELECTRIC_H
#define ELECTRIC_H
#include "bullet.h"

class Bullet2:public Bullet{
    Q_OBJECT
    Q_PROPERTY(QPoint m_currentPos READ currentPos WRITE setCurrentPos)
public:
    Bullet2(QPoint startPos, QPoint targetPoint, int damage, Enemy *target,MainWindow *game,QPixmap sprite);
    void peratk(Enemy *enemy);
public slots:
    void hitTarget();
};

#endif // ELECTRIC_H
