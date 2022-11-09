#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "waypoint.h"
#include "enemy.h"
#include "bullet.h"
#include "audioplayer.h"
#include "icon.h"
#include "res.h"
#include "tower.h"
#include "tower1.h"
#include "tower2.h"
#include "tower3.h"
#include "maintower.h"
#include "boss.h"
#include <QPainter>
#include <QThread>
#include <QMouseEvent>
#include<QKeyEvent>
#include <QtGlobal>
#include <QMessageBox>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QTimer>
#include <QXmlStreamReader>
#include <QtDebug>
#include <QDebug>
#include <QMediaPlayer>
#include <QMediaPlaylist>


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
    , m_playerHp(30)
    , m_playrGold(800)
    , m_gameEnded(false)
	, m_gameWin(false)
    , compete(false)
    , player2Gold(400)
{
	ui->setupUi(this);
     setFixedSize(1900,950);
	QTimer *timer = new QTimer(this);
    competeTimer = new QTimer(this);
    lefttimer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateMap()));
    timer->start(30);
    connect(competeTimer,SIGNAL(timeout()),this, SLOT(competeShift()));
    connect(lefttimer, SIGNAL(timeout()), this, SLOT(on_time_out()));

    QMediaPlaylist * playlist = new QMediaPlaylist;
    playlist->addMedia(QUrl("qrc:/music/bgm.mp3"));
    playlist->addMedia(QUrl("qrc:/music/bgm.mp3"));
    playlist->setCurrentIndex(1);
    playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    QMediaPlayer * player = new QMediaPlayer;
    player->setPlaylist(playlist);
    player->setVolume(100);
    player->play();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::loadTowerPositions()
{
    if(stage == 1){
        m_towerPositionsList.push_back(QPoint(386,578));
        m_towerPositionsList.push_back(QPoint(481,384));
        m_towerPositionsList.push_back(QPoint(481,193));
        m_towerPositionsList.push_back(QPoint(763,3));
        m_towerPositionsList.push_back(QPoint(958,188));
        m_towerPositionsList.push_back(QPoint(1343,384));
}
    else if(stage == 2){
        m_towerPositionsList.push_back(QPoint(1050,384));
        m_towerPositionsList.push_back(QPoint(866,384));
        m_towerPositionsList.push_back(QPoint(866,186));
        m_towerPositionsList.push_back(QPoint(384,186));
        m_towerPositionsList.push_back(QPoint(384,384));
        m_towerPositionsList.push_back(QPoint(201,670));
    }
    else if(stage == 3){
        m_towerPositionsList.push_back(QPoint(772,289));
        m_towerPositionsList.push_back(QPoint(1058,290));
        m_towerPositionsList.push_back(QPoint(1058,484));
        m_towerPositionsList.push_back(QPoint(389,484));
        m_towerPositionsList.push_back(QPoint(673,484));
        m_towerPositionsList.push_back(QPoint(1250,386));
    }




}

void MainWindow::paintEvent(QPaintEvent *)
{
    if(!m_gamestart){
        QPainter painter(this);
        QPixmap start(":/image/wiki_bg_center.png");
        painter.drawPixmap(0,0,start);
    }
    else{
        if (m_gameEnded || m_gameWin)
	{
        QPixmap l;
        if(m_gameEnded)l.load (":/image/lost.png");
        else l.load (":/image/win.png");

		QPainter painter(this);
        painter.drawPixmap(0, 0, l);
		return;
	}
    if(m_gameShift){
        if(compete){
            competeTimer->stop();
        }
        QPixmap s;
        if(stage == 2)s.load(":/image/shift1.png");
        else if(stage == 3)s.load(":/image/shift2.png");
        QPainter painter(this);
        painter.drawPixmap(0, 0, s);
    }
    else{
    QPixmap t;
    if(stage == 1){
    QPixmap cachePix(":/image/tilemap2.png");
    t = cachePix;
    }
    else if (stage == 2) {
        QPixmap cachePix(":/image/tilemap3.png");
        t = cachePix;
    }
    else if(stage == 3){
        QPixmap cachePix(":/image/tilemap0.png");
        t= cachePix;
    }
    QPainter cachePainter(&t);
	foreach (const TowerPosition &towerPos, m_towerPositionsList)
		towerPos.draw(&cachePainter);

    foreach (const Tower *tower, m_towersList)
		tower->draw(&cachePainter);
	foreach (const Enemy *enemy, m_enemyList)
		enemy->draw(&cachePainter);

	foreach (const Bullet *bullet, m_bulletList)
		bullet->draw(&cachePainter);

	QPainter painter(this);
    painter.drawPixmap(0, 0, t);
    }
    QPainter painter(this);
    drawMainTowerLevel(&painter);
    drawWave(&painter);
    drawHP(&painter);
    drawPlayerGold(&painter);
    if(compete) drawPlayer2Gold(&painter);
    if(compete) drawTimeLeft(&painter);
    }
}

void MainWindow::information_of_enemy(int waves)
{
    int number_of_enemy[5] = {3, 2, 0, 0, 0};
    int number_of_enemy1[5] = {2, 3, 4, 2, 0};
    int number_of_enemy2[5] = {0, 1, 2, 4, 4};
    int number_of_enemy3[5] = {0, 0, 1, 2, 4};
    ui->plainTextEdit->clear();
    QFont font("微软雅黑",12,Qt::black);
    ui->plainTextEdit->setFont(font);
    ui->plainTextEdit->appendPlainText("本波敌人信息:\n");
    ui->plainTextEdit->appendPlainText(QString("三体雇佣兵   数量 : %1").arg(number_of_enemy[waves]));
    ui->plainTextEdit->appendPlainText("三体雇佣的人类士兵，各项属性较为均衡");
    ui->plainTextEdit->appendPlainText(QString("三体精锐步兵  数量 : %1").arg(number_of_enemy1[waves]));
    ui->plainTextEdit->appendPlainText("经过三体人魔鬼训练的士兵，射程、攻击力、移速均有所上升，但更加脆弱");
    ui->plainTextEdit->appendPlainText(QString("三体人型自走机甲    数量 : %1").arg(number_of_enemy2[waves]));
    ui->plainTextEdit->appendPlainText("三体研制的一种让使用者身体素质大幅提高的机甲，但移动较为缓慢");
    ui->plainTextEdit->appendPlainText(QString("三体改良机甲  数量 : %1").arg(number_of_enemy3[waves]));
    ui->plainTextEdit->appendPlainText("在原有机甲的基础之上，使用了更加先进的武器以及引擎，但牺牲了装甲的厚度");
    ui->plainTextEdit->appendPlainText("请提前做好准备！");

    QApplication::processEvents();
}

void MainWindow::judgetow0()
{
    p = 0;
}
void MainWindow::judgetow1()
{
    p = 1;
}
void MainWindow::judgetow2()
{
    p = 2;
}
void MainWindow::judgetow3()
{
    p = 3;
}
void MainWindow::ifup(){
    flag = 1;
}
void MainWindow::ifun(){
    flag = 0;
}


void MainWindow::setMenu(){
    QTMyIconStyle *m_pMyStyle = new QTMyIconStyle();
    setmenu = new QMenu(this);
    setmenu->setStyle(m_pMyStyle);
    tow = new QAction(setmenu);
    tow->setText("青铜时代号($200):常规性战舰，各项属性均衡，有很大的升级潜力");
    tow->setIcon(QIcon(":/image/tower_4_select.png"));
    tow1 = new QAction(setmenu);
    tow1->setText("蓝色空间号($250)：配备了范围伤害的加农炮，但速率较慢");
    tow1->setIcon(QIcon(":/image/tower_7_select.png"));
    tow2 = new QAction(setmenu);
    tow2->setText("自然选择号($300)：使用了生化武器，可以基于敌人生命值伤害");
    tow2->setIcon(QIcon(":/image/tower_10_select.png"));
    tow3 = new QAction(setmenu);
    tow3->setText("魔法水晶炮($300)：使用了神秘的膜法力量，可以对敌人暂时造成减速");
    tow3->setIcon(QIcon(":/image/wuqi.png"));
    setmenu->addAction(tow);
    setmenu->addAction(tow1);
    setmenu->addAction(tow2);
    setmenu->addAction(tow3);
    connect(tow,SIGNAL(triggered()),this,SLOT(judgetow0()));
    connect(tow1,SIGNAL(triggered()),this,SLOT(judgetow1()));
    connect(tow2,SIGNAL(triggered()),this,SLOT(judgetow2()));
    connect(tow3,SIGNAL(triggered()),this,SLOT(judgetow3()));

    upmenu = new QMenu(this);
    upmenu->setStyle(m_pMyStyle);
    up = new QAction(upmenu);
    up->setText("升级：综合提升战舰性能");
    up->setIcon(QIcon(":/image/crit.png"));
    un = new QAction(this);
    un->setText("出售：返还总建造费用的60%");
    un->setIcon(QIcon(":/image/Coin.png"));
    upmenu->addAction(up);
    upmenu->addAction(un);
    connect(up,SIGNAL(triggered()),this,SLOT(ifup()));
    connect(un,SIGNAL(triggered()),this,SLOT(ifun()));

}
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint pressPos = event->pos();
    if(!m_gamestart&&pressPos.x()>=1405&&pressPos.x()<=1536&&pressPos.y()>=0&&pressPos.y()<=113)
    {
        QMessageBox message(QMessageBox::NoIcon, "背景介绍", "在超新星爆发后，给附近几个星系带来了几个世纪内几乎用不完的能量，但是也点燃了这附近的黑暗森林，附近的几个星系都知道了对方的存在，一场星球大战一触即发。");
        message.setIconPixmap(QPixmap(":/image/info.png"));
        message.exec();
    }
    if(!m_gamestart&&pressPos.x()>=1004&&pressPos.x()<=1252&&pressPos.y()>=239&&pressPos.y()<=486){
        m_gamestart = true;
        gameStart();//开始界面
    }
    if(!m_gamestart&&pressPos.x() >=1004 && pressPos.x() <= 1252 && pressPos.y() >= 530 && pressPos.y() <= 776){
        m_gamestart = true;
        compete = true;
        gameStart();//对抗模式
    }
    if(m_gameShift&&pressPos.x()>=0&&pressPos.x()<=240&&pressPos.y()>=650&&pressPos.y()<=864){
        m_gameShift = false;
        gameStart();//切换
    }
    for(auto it = m_towerPositionsList.begin();it !=m_towerPositionsList.end();it++)
    {
        if (it->containPoint(pressPos) && !it->hasTower()){
                        setmenu->popup(pressPos);
                        setmenu->exec(QCursor().pos());
                        if(p == 0){
                            if(m_playrGold>=200){Tower *tower = new Tower(it->centerPos(), this);
                                m_towersList.push_back(tower);
                                m_playrGold -= 200;}
                            else QMessageBox::warning(this,"错误","您的金钱不够");
                        }
                        else if(p == 1){
                            if(m_playrGold>=250){
                            Tower1 *tower = new Tower1(it->centerPos(), this);
                            m_towersList.push_back(tower);
                            m_playrGold -= 250;
                            }
                            else QMessageBox::warning(this,"错误","您的金钱不够");
                        }
                        else if(p == 2){
                            if(m_playrGold>=300){
                            Tower2 *tower = new Tower2(it->centerPos(), this);
                            m_towersList.push_back(tower);
                            m_playrGold -= 300;
                            }
                            else QMessageBox::warning(this,"错误","您的金钱不够");
                        }
                        else if(p == 3){
                            if(m_playrGold>=300){
                            Tower3 *tower = new Tower3(it->centerPos(), this);
                            m_towersList.push_back(tower);
                            m_playrGold -= 300;
                            }
                            else QMessageBox::warning(this,"错误","您的金钱不够");
                        }
                        else break;
                        p = -1;
                        it->setHasTower();
                        update();
                        break;
        }
        else if(it->hasTower()&&it->containPoint(pressPos)){
            for (auto t = m_towersList.begin();t != m_towersList.end();t++) {
                if((*t)->contain(pressPos)){
                    upmenu->popup(pressPos);
                    upmenu->exec(QCursor().pos());
                        if(flag == 1){
                            if((*t)->uplevel<= (m->uplevel-1)){
                                if(m_playrGold>=(*t)->upcost){
                                    m_playrGold -= (*t)->upcost;
                                    (*t)->upgrade();

                                }
                                else QMessageBox::warning(this,"错误","您的金钱不够");
                            }
                            else QMessageBox::warning(this,"错误","防御塔等级不能高于基地，请先升级基地");
                        }
                        else if(flag == 0){
                            m_playrGold += (*t)->discost;
                            (*t)->uninstall();
                            it->setNoTower();
                            m_towersList.removeOne(*t);
                        }
                        else break;
                        flag = -1;
                        update();
                        break;
                    }
                }

            }

}
}

void MainWindow::competeShift()
{
    for(auto t = m_towersList.begin();t != m_towersList.end();t ++){
        (*t)->inExist();
    }
    if(stage < 3){
        player2Gold = 400;//刷新新关卡后player2的金钱重置
        stage ++;
        m_gameShift = true;
        for (auto t = m_enemyList.begin();t != m_enemyList.end();t++){
            this->removedEnemy(*t);
        }
        loadStage();
    }else{
        if(!m_gameWin && !m_gameEnded){
            m_gameWin = true;
            for (auto t = m_enemyList.begin();t != m_enemyList.end();t++){
                this->removedEnemy(*t);
            }
            competeTimer->stop();
        }
    }
     qDebug() << "out";
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(!m_gameShift && m_gamestart  && !m_gameWin){
        WayPoint *startWayPoint = m_wayPointsList.back();
        int consume0 = 200,consume1 = 250,consume2 = 400,consume3 = 450,consume4 = 1000;
        if(event->key() == Qt::Key_M){
            m_playrGold+=200;
        }
        if(event->key() == Qt::Key_N&&compete){
            player2Gold+=200;
        }
        if(event->key() == Qt::Key_Q && compete){
            if( player2Gold >= consume0){
            player2Gold -= consume0;
            Enemy*enemy = new Enemy(startWayPoint,this);
            m_enemyList.push_back(enemy);
            enemy->doActivate();
            }
            else QMessageBox::warning(this,"错误","您的结晶不够");

        }else if(event->key() == Qt::Key_W && compete){
            if(player2Gold >= consume1){
            player2Gold -= consume1;
            Enemy*enemy = new Enemy1(startWayPoint,this);
            m_enemyList.push_back(enemy);
            enemy->doActivate();
            }
            else QMessageBox::warning(this,"错误","您的结晶不够");

        }else if(event->key() == Qt::Key_E && compete){
            if(player2Gold>=consume2){
            player2Gold -= consume2;
            Enemy*enemy = new Enemy2(startWayPoint,this);
            m_enemyList.push_back(enemy);
            enemy->doActivate();
            }
            else QMessageBox::warning(this,"错误","您的结晶不够");

        }else if(event->key() == Qt::Key_R&& compete){
            if(player2Gold >= consume3){
            player2Gold -= consume3;
            Enemy*enemy = new Enemy3(startWayPoint,this);
            m_enemyList.push_back(enemy);
            enemy->doActivate();
            }
            else QMessageBox::warning(this,"错误","您的结晶不够");

        }
        else if(event->key() == Qt::Key_T&& compete){
            if(player2Gold >= consume4){
                    player2Gold -= consume4;
                    Enemy*enemy = new Boss(startWayPoint,this);
                    m_enemyList.push_back(enemy);
                    enemy->doActivate();
            }
            else QMessageBox::warning(this,"错误","您的结晶不够");

                }
    }
}

void MainWindow::drawMainTowerLevel(QPainter *painter)
{
    painter->setPen(QPen(Qt::black));
    painter->drawText(QRect(1600, 20, 100, 25), QString("基地等级 : %1").arg(m->uplevel));
    painter->drawText(QRect(1700, 20, 200, 25), QString("升级费用 : %3").arg(m->upcost));
    painter->drawPixmap(1540, 3, 50, 50, QPixmap(":/image/level.jpg"));
}

void MainWindow::drawWave(QPainter *painter)
{
    painter->setPen(QPen(Qt::black));
    painter->drawText(QRect(1600, 80, 100, 25), QString("波数 : %1").arg(m_waves + 1));
    painter->drawPixmap(1540, 63, 50, 50, QPixmap(":/image/wave.jpg"));
}

void MainWindow::drawHP(QPainter *painter)
{
    painter->setPen(QPen(Qt::black));
    painter->drawText(QRect(1600, 140, 100, 25), QString("生命值 : %1").arg(m_playerHp));
    painter->drawPixmap(1540, 123, 50, 50, QPixmap(":/image/hp.jpg"));
}

void MainWindow::drawPlayerGold(QPainter *painter)
{
    painter->setPen(QPen(Qt::black));
    painter->drawText(QRect(1600, 200, 100, 25), QString("金钱 : %1").arg(m_playrGold));
    painter->drawPixmap(1540, 183, 50, 50, QPixmap(":/image/gold.png"));
}

void MainWindow::drawPlayer2Gold(QPainter *painter)
{
    painter->setPen(QPen(Qt::black));
    painter->drawText(QRect(1600, 260, 200, 25), QString("结晶 : %1").arg(player2Gold));
    painter->drawPixmap(1540, 243, 50, 50, QPixmap(":/image/Diamond.png"));
}

void MainWindow::drawTimeLeft(QPainter *painter)
{
    painter->setPen(QPen(Qt::black));
    painter->drawText(QRect(1600, 320, 200, 25), QString("剩余时间 : %1").arg(time_left));
    painter->drawPixmap(1540, 303, 50, 50, QPixmap(":/image/time.png"));
}

void MainWindow::doGameOver()
{
	if (!m_gameEnded)
	{
		m_gameEnded = true;
	}
}

void MainWindow::awardGold(int gold)
{
	m_playrGold += gold;
	update();
}

void MainWindow::addWayPoints()
{
    if(stage == 1){
    WayPoint *wayPoint1 = new WayPoint(QPoint(1294, 670));
	m_wayPointsList.push_back(wayPoint1);

    WayPoint *wayPoint2 = new WayPoint(QPoint(1294, 239));
	m_wayPointsList.push_back(wayPoint2);
	wayPoint2->setNextWayPoint(wayPoint1);

    WayPoint *wayPoint3 = new WayPoint(QPoint(1102, 239));
	m_wayPointsList.push_back(wayPoint3);
	wayPoint3->setNextWayPoint(wayPoint2);

    WayPoint *wayPoint4 = new WayPoint(QPoint(1102, 142));
	m_wayPointsList.push_back(wayPoint4);
	wayPoint4->setNextWayPoint(wayPoint3);

    WayPoint *wayPoint5 = new WayPoint(QPoint(427, 142));
    m_wayPointsList.push_back(wayPoint5);
    wayPoint5->setNextWayPoint(wayPoint4);

    WayPoint *wayPoint6 = new WayPoint(QPoint(427, 334));
    m_wayPointsList.push_back(wayPoint6);
    wayPoint6->setNextWayPoint(wayPoint5);

    WayPoint *wayPoint7 = new WayPoint(QPoint(717, 334));
    m_wayPointsList.push_back(wayPoint7);
    wayPoint7->setNextWayPoint(wayPoint6);

    WayPoint *wayPoint8 = new WayPoint(QPoint(717, 524));
    m_wayPointsList.push_back(wayPoint8);
    wayPoint8->setNextWayPoint(wayPoint7);

    WayPoint *wayPoint9 = new WayPoint(QPoint(335, 524));
    m_wayPointsList.push_back(wayPoint9);
    wayPoint9->setNextWayPoint(wayPoint8);

    WayPoint *wayPoint10 = new WayPoint(QPoint(335, 764));
    m_wayPointsList.push_back(wayPoint10);
    wayPoint10->setNextWayPoint(wayPoint9);
    }
    else if (stage == 2) {
        WayPoint *wayPoint1 = new WayPoint(QPoint(710,719));
        m_wayPointsList.push_back(wayPoint1);

        WayPoint *wayPoint2 = new WayPoint(QPoint(336,719));
        m_wayPointsList.push_back(wayPoint2);
        wayPoint2->setNextWayPoint(wayPoint1);

        WayPoint *wayPoint3 = new WayPoint(QPoint(336,336));
        m_wayPointsList.push_back(wayPoint3);
        wayPoint3->setNextWayPoint(wayPoint2);

        WayPoint *wayPoint4 = new WayPoint(QPoint(528,336));
        m_wayPointsList.push_back(wayPoint4);
        wayPoint4->setNextWayPoint(wayPoint3);

        WayPoint *wayPoint5 = new WayPoint(QPoint(528,242));
        m_wayPointsList.push_back(wayPoint5);
        wayPoint5->setNextWayPoint(wayPoint4);

        WayPoint *wayPoint6 = new WayPoint(QPoint(815,242));
        m_wayPointsList.push_back(wayPoint6);
        wayPoint6->setNextWayPoint(wayPoint5);

        WayPoint *wayPoint7 = new WayPoint(QPoint(815,531));
        m_wayPointsList.push_back(wayPoint7);
        wayPoint7->setNextWayPoint(wayPoint6);

        WayPoint *wayPoint8 = new WayPoint(QPoint(1198,531));
        m_wayPointsList.push_back(wayPoint8);
        wayPoint8->setNextWayPoint(wayPoint7);

        WayPoint *wayPoint9 = new WayPoint(QPoint(1198,195));
        m_wayPointsList.push_back(wayPoint9);
        wayPoint9->setNextWayPoint(wayPoint8);
    }
    else if (stage == 3) {
        WayPoint *wayPoint1 = new WayPoint(QPoint(336,300));
        m_wayPointsList.push_back(wayPoint1);

        WayPoint *wayPoint2 = new WayPoint(QPoint(336,622));
        m_wayPointsList.push_back(wayPoint2);
        wayPoint2->setNextWayPoint(wayPoint1);

        WayPoint *wayPoint3 = new WayPoint(QPoint(1197,622));
        m_wayPointsList.push_back(wayPoint3);
        wayPoint3->setNextWayPoint(wayPoint2);

        WayPoint *wayPoint4 = new WayPoint(QPoint(1197,242));
        m_wayPointsList.push_back(wayPoint4);
        wayPoint4->setNextWayPoint(wayPoint3);

        WayPoint *wayPoint5 = new WayPoint(QPoint(1008,242));
        m_wayPointsList.push_back(wayPoint5);
        wayPoint5->setNextWayPoint(wayPoint4);

        WayPoint *wayPoint6 = new WayPoint(QPoint(1008,430));
        m_wayPointsList.push_back(wayPoint6);
        wayPoint6->setNextWayPoint(wayPoint5);

        WayPoint *wayPoint7 = new WayPoint(QPoint(716,430));
        m_wayPointsList.push_back(wayPoint7);
        wayPoint7->setNextWayPoint(wayPoint6);

        WayPoint *wayPoint8 = new WayPoint(QPoint(716,192));
        m_wayPointsList.push_back(wayPoint8);
        wayPoint8->setNextWayPoint(wayPoint7);
    }
}

void MainWindow::getHpDamage(int damage)
{
    QMediaPlayer * player = new QMediaPlayer;
    player->setMedia(QUrl("qrc:/music/life_lose.wav"));
    player->setVolume(80);
    player->play();
	m_playerHp -= damage;
    player2Gold += damage * 50;//赏金系统：嗜血
	if (m_playerHp <= 0)
		doGameOver();
}

void MainWindow::removedEnemy(Enemy *enemy)
{
	Q_ASSERT(enemy);
	m_enemyList.removeOne(enemy);
	delete enemy;

    if (m_enemyList.empty() && !compete)
	{
		++m_waves;
		if (!loadWave())
		{
			m_gameWin = true;
		}

     }

}

void MainWindow::removedBullet(Bullet *bullet)
{
	Q_ASSERT(bullet);

	m_bulletList.removeOne(bullet);
	delete bullet;
}

void MainWindow::addBullet(Bullet *bullet)
{
	Q_ASSERT(bullet);

	m_bulletList.push_back(bullet);
}

void MainWindow::updateMap()
{
    if(m_gamestart && !m_gameShift){
        player2Gold += 1;
    }
	foreach (Enemy *enemy, m_enemyList)
		enemy->move();
	foreach (Tower *tower, m_towersList)
		tower->checkEnemyInRange();
	update();
}

void MainWindow::loadMain(){
    if(stage == 1){
        m = new MainTower(QPoint(1294, 670), this);
        m_towersList.push_back(m);
    }
    else if(stage == 2){
        m = new MainTower(QPoint(718,719), this);
        m_towersList.push_back(m);
    }
    else if(stage == 3){
        m = new MainTower(QPoint(336,288), this);
        m_towersList.push_back(m);
    }

}
void MainWindow::loadStage(){
        m_towerPositionsList.clear();
        m_towersList.clear();
        m_wayPointsList.clear();
        m_bulletList.clear();


}
bool MainWindow::loadWave()
{
    if(stage == 3 && m_waves >= 5)return false;
    else if (stage <3 &&m_waves >= 5){
        stage++;
        m_waves = 0;
       m_gameShift = true;

    }
    else {
        WayPoint *startWayPoint = m_wayPointsList.back(); // 这里是个逆序的，尾部才是其实节点
        if(m_waves == 0){
            int enemyStartInterval[] = {1000,3000,5000,7000,9000};
            for(int i = 0;i < 5; ++ i){
                Enemy *enemy;
                if( i<=2 )  enemy = new Enemy(startWayPoint,this);
                else   enemy = new Enemy1(startWayPoint,this);
                m_enemyList.push_back(enemy);
                QTimer::singleShot(enemyStartInterval[i],enemy,SLOT(doActivate()));
                information_of_enemy(m_waves);
            }
        }
        else if(m_waves == 1){
            int enemyStartInterval[] = {1000,3000,5000,7000,9000,12000};
            for(int i = 0;i < 6; ++ i){
                Enemy *enemy;
                if(i == 0 || i == 1) enemy = new Enemy(startWayPoint,this);
                else if(i>=2 && i<=4) enemy = new Enemy1(startWayPoint,this);
                else enemy = new Enemy2(startWayPoint,this);
                m_enemyList.push_back(enemy);
                QTimer::singleShot(enemyStartInterval[i],enemy,SLOT(doActivate()));
                information_of_enemy(m_waves);
            }
        }
            else if(m_waves == 2){
            int enemyStartInterval[] = {1000,3000,5000,7000,9000,12000,14000};
            for(int i = 0;i < 7; ++ i){
                Enemy *enemy;
                if(i == 0 || i == 1) enemy = new Enemy2(startWayPoint,this);
                else if(i == 6) enemy = new Enemy3(startWayPoint,this);
                else enemy = new Enemy1(startWayPoint,this);
                m_enemyList.push_back(enemy);
                QTimer::singleShot(enemyStartInterval[i],enemy,SLOT(doActivate()));
                information_of_enemy(m_waves);
            }
         }else if(m_waves == 3){
                int enemyStartInterval[] = {1000,3000,5000,7000,9000,12000,14000,16000};
                for(int i = 0;i < 8; ++ i){
                    Enemy *enemy;
                    if(i == 0 || i == 1) enemy = new Enemy1(startWayPoint,this);
                    else if(i == 6 || i == 7) enemy = new Enemy3(startWayPoint,this);
                    else enemy = new Enemy2(startWayPoint,this);
                    m_enemyList.push_back(enemy);
                    QTimer::singleShot(enemyStartInterval[i],enemy,SLOT(doActivate()));
                    information_of_enemy(m_waves);
                }
        }else if(m_waves == 4){
            QMessageBox::warning(this,"警告","Boss来袭！Boss血量极厚，请做好准备！");
            int enemyStartInterval[] = {1000,3000,5000,7000,9000,12000,14000,16000,18000};
            for(int i = 0;i < 9; ++ i){
                Enemy *enemy;
                if( i >= 0&&i<= 3) enemy = new Enemy2(startWayPoint,this);
                else if( i >= 4&&i<= 7) enemy = new Enemy3(startWayPoint,this);
                else enemy = new Boss(startWayPoint,this);
                m_enemyList.push_back(enemy);
                QTimer::singleShot(enemyStartInterval[i],enemy,SLOT(doActivate()));
                information_of_enemy(m_waves);
            }
        }
}
    return true;
}

QList<Enemy *> MainWindow::enemyList() const
{
	return m_enemyList;
}

void MainWindow::on_time_out(){
    --time_left;
    if(time_left == 0)
    {
        lefttimer->stop();
        time_left = 60;
    }
}

void MainWindow::Key_information(){


        QFont font("微软雅黑",12,Qt::black);
        ui->plainTextEdit->setFont(font);
        ui->plainTextEdit->appendPlainText("对抗模式游戏说明：");
        ui->plainTextEdit->appendPlainText("对抗模式分为三轮，每轮时间为60秒。在规定时间内，鼠标玩家通过花费金钱建造炮塔进行防御，键盘玩家通过使用'Q'、'W'、'E'、'R'四个键花费金钱增加敌人进行进攻。若鼠标玩家在规定时间内生存下来，则鼠标玩家胜利。本次游戏键盘玩家为Player2，玩家2金币为赏金机制，时间没减少1秒玩家2金币会增加100。");
        ui->plainTextEdit->appendPlainText("键盘玩家游戏说明：");
        ui->plainTextEdit->appendPlainText("Q键为 三体雇佣兵       花费金币200");
        ui->plainTextEdit->appendPlainText("W键为 三体精锐步兵     花费金币250");
        ui->plainTextEdit->appendPlainText("E键为 三体人型自走机甲  花费金币400");
        ui->plainTextEdit->appendPlainText("W键为 三体改良机甲     花费金币450");
        ui->plainTextEdit->appendPlainText("T键为 天启太空坦克     花费金币1000");
        QApplication::processEvents();

}

void MainWindow::gameStart()
{
    if(stage>1){
        loadStage();
    }
    loadMain();
    loadTowerPositions();
    addWayPoints();
    setMenu();
    if(!compete){
        loadWave();
    }
    if(compete){
        int time = 60;
        competeTimer->start(1000*time);
        lefttimer->start(1000);
        Key_information();
    }
}

void MainWindow::on_pushButton_clicked()
{
    if(m_gamestart){
      if((m)->uplevel<3 ){
          if(m_playrGold>=(m)->upcost){
        m_playrGold -= (m)->upcost;
       (m)->upgrade();
        m_playerHp+=5;
          }
          else QMessageBox::warning(this,"错误","您的金钱不够");
}
      else QMessageBox::warning(this,"错误","您的基地已为最高级别");
    }
    else QMessageBox::warning(this,"错误","游戏尚未开始");
}

void MainWindow::on_pushButton_2_clicked()
{
    if(m_gamestart){
        if(m_playrGold>=3000){
        for (auto t = m_towersList.begin();t != m_towersList.end();t++) {
            (*t)->m_fireRate-=400;
            (*t)->m_damage+=3;
        }
        ui->pushButton_2->setEnabled(false);
        }
        else QMessageBox::warning(this,"错误","您的金钱不够");
    }
    else QMessageBox::warning(this,"错误","游戏尚未开始");


}


void MainWindow::on_pushButton_3_clicked()
{
    if(m_gamestart){
        if(m_playrGold>=2000){
            for (auto t = m_enemyList.begin();t != m_enemyList.end();t++) {
                (*t)->m_walkingSpeed = 0.6*(*t)->m_walkingSpeed;
            }
        ui->pushButton_3->setEnabled(false);
        }
        else QMessageBox::warning(this,"错误","您的金钱不够");
    }
    else QMessageBox::warning(this,"错误","游戏尚未开始");


}

void MainWindow::on_pushButton_4_clicked()
{
    if(m_gamestart){
        foreach(Tower* tower,m_towersList){
            tower->uninstall();
        }
        m_enemyList.clear();
        stage++;
        m_waves = 0;
        gameStart();
        if(stage == 3)ui->pushButton_4->setEnabled(false);

    }
    else QMessageBox::warning(this,"错误","游戏尚未开始");



}

void MainWindow::on_pushButton_5_clicked()
{
    if(m_gamestart){
        if(m_playrGold>=2000){
            fire  = 1;
            ui->pushButton_5->setEnabled(false);
        }
        else QMessageBox::warning(this,"错误","您的金钱不够");
    }
    else QMessageBox::warning(this,"错误","游戏尚未开始");

}
