#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "towerposition.h"
#include "tower.h"



namespace Ui {
class MainWindow;
}

class WayPoint;
class Enemy;
class Bullet;
class AudioPlayer;


class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
    void getHpDamage(int damage);
	void removedEnemy(Enemy *enemy);
	void removedBullet(Bullet *bullet);
	void addBullet(Bullet *bullet);
	void awardGold(int gold);
    QList<Enemy *>			m_enemyList;
	QList<Enemy *> enemyList() const;

protected:
	void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *);

private slots:
	void updateMap();
    void judgetow0();
    void judgetow1();
    void judgetow2();
    void judgetow3();

    void ifup();
    void ifun();
    void gameStart();
    void information_of_enemy(int waves);
    void Key_information();
    void on_time_out();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();
    void competeShift();

    void on_pushButton_5_clicked();

private:
	void loadTowerPositions();
	void addWayPoints();
	bool loadWave();
	void drawWave(QPainter *painter);
	void drawHP(QPainter *painter);
	void drawPlayerGold(QPainter *painter);
    void drawPlayer2Gold(QPainter *painter);
    void drawMainTowerLevel(QPainter *painter);
    void drawTimeLeft(QPainter *painter);
    void setMenu();


	void doGameOver();
    void loadMain();
    void loadStage();
    QMenu *setmenu;
    QMenu *upmenu;
    QMenu *mainmenu;
    QAction *tow;
    QAction *tow1;
    QAction *tow2;
    QAction *tow3;

    QAction *up;
    QAction *un;
    int p = -1;
    int flag = -1;
    int time_left = 60;

	Ui::MainWindow *		ui;
	int						m_playerHp;
	int						m_playrGold;
	bool					m_gameEnded;
	bool					m_gameWin;
    bool                    m_gamestart = false;
    bool                    m_gameShift = false;
	QList<QVariant>			m_wavesInfo;
	QList<TowerPosition>	m_towerPositionsList;
	QList<Tower *>			m_towersList;
	QList<WayPoint *>		m_wayPointsList;
	QList<Bullet *>			m_bulletList;
    QTimer *competeTimer;
    QTimer *lefttimer;
    bool nextwave = false;
    bool compete;//对抗模式
    int player2Gold;
    QPoint mainlist[3] = {
        QPoint(1294, 670),
        QPoint(574,719),
        QPoint(336,384)
    };
    Tower* m;
};

#endif // MAINWINDOW_H
