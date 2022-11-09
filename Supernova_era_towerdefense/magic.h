#ifndef MAGIC_H
#define MAGIC_H
#include "bullet.h"


class Bullet3:public Bullet{
    Q_OBJECT
    Q_PROPERTY(QPoint m_currentPos READ currentPos WRITE setCurrentPos)
public:
    Bullet3(QPoint startPos, QPoint targetPoint, int damage, Enemy *target,MainWindow *game,QPixmap sprite);
    void slowenemy(Enemy *enemy);
public slots:
    void hitTarget();
};


#endif // MAGIC_H
