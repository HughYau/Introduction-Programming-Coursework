#ifndef BULLETTOTOWER_H
#define BULLETTOTOWER_H
#include "bullet.h"

class BulletToTower : public Bullet{
    Q_OBJECT
    Q_PROPERTY(QPoint m_currentPos READ currentPos WRITE setCurrentPos)
public:
    BulletToTower(QPoint startPos, QPoint targetPoint, int damage, Enemy *target,MainWindow *game,QPixmap sprite);
public slots:
    void hitTarget();
};

#endif // BULLETTOTOWER_H
