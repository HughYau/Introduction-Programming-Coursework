#ifndef BOMB_H
#define BOMB_H
#include "bullet.h"


class Bullet1:public Bullet{
    Q_OBJECT
    Q_PROPERTY(QPoint m_currentPos READ currentPos WRITE setCurrentPos)
public:
    Bullet1(QPoint startPos, QPoint targetPoint, int damage, Enemy *target,MainWindow *game,QPixmap sprite);
    void rangeatk();
public slots:
    void hitTarget();
};

#endif // BOMB_H
