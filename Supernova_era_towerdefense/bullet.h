#ifndef BULLET_H
#define BULLET_H

#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <QObject>

class QPainter;
class Enemy;
class MainWindow;

class Bullet : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QPoint m_currentPos READ currentPos WRITE setCurrentPos)

public:
	Bullet(QPoint startPos, QPoint targetPoint, int damage, Enemy *target,
           MainWindow *game, QPixmap sprite);

	void draw(QPainter *painter) const;
	void setCurrentPos(QPoint pos);
	QPoint currentPos() const;
    virtual void move();
    int             m_range;
    int             m_ratk;
    double             m_per;
    double             m_speedrate;
protected:
    QPixmap	m_sprite;
    const QPoint	m_startPos;
    const QPoint	m_targetPos;
    QPoint			m_currentPos;
    Enemy *			m_target;
    MainWindow *	m_game;
    int				m_damage;
    static const QSize IconSize;



protected slots:
	void hitTarget();

};

#endif // BULLET_H
