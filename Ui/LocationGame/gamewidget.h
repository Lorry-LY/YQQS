#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QKeyEvent>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "./stopwidget.h"
#include "../option/settingwidget.h"
#include "controller/mediaController/mediacontroller.h"
#include "controller/LocationGame/bulletcontroller.h"
#include "controller/LocationGame/charactercontroller.h"
#include "controller/LocationGame/monstercontroller.h"
#include "controller/LocationGame/propcontroller.h"
#include "controller/LocationGame/viewwidget.h"
#include "controller/LocationGame/thread/hitcharacterthread.h"
#include "controller/LocationGame/thread/hitmonsterthread.h"
#include "controller/LocationGame/thread/hitbarrierthread.h"
#include "controller/LocationGame/thread/monsterthread.h"
#include "controller/LocationGame/thread/pickthingsthread.h"


namespace Ui {
class GameWidget;
}

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = nullptr);
    ~GameWidget();

    void init(int character);

    void continueGame(int number,int gameTime,int characterID,int HP,int MP,int coin,int weaponID);

    void dealGameOver();
    void dealGameStop();
    void dealGameContinue();
    void saveGameState();
    void updateGame();
    void updateMap();
    void dealMonsterMove(int monsterID,char moveWay);
    void dealMonsterShoot(MonsterBullet* bullet);
    void dealMonsterWeaponAngle(int monsterID,float angle);
    void dealCharacterShoot(Character* character,Bullet* bullet);
    void setHP(int hp, int maxHp);
    void setMP(int mp, int maxMp);
    void setArmor(int armor, int maxArmor);
    StopWidget *getStop();
    QVector<QString> getInformation();

signals:
    void StopGame();
    void die(int time,QPixmap pixmap,int GoldNum,int Level,QString Result,QString characterName,QString weaponName,int coin);
    void saveGame();
private slots:
    void dealStopGame();
    void dealContinueGame();
    void dealSetting();
    void dealFinished();
    void dealChangeBGMvolume(int volumn);
    void dealChangeSoundVolume(int volumn);
    void dealChangeWeapon(Weapon* oldWeapon);
    void changeView(QPoint center);


    void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void keyReleaseEvent(QKeyEvent *event);

    QStringList getFileNames(const QString &path);

    void on_PushButton_Parse_released();

    void setMoney(int money);

    void transitionAnimation();

private:
    Ui::GameWidget *ui;

    StopWidget *stop;
    SettingWidget *setting;
    MediaController *media;

    CharacterController* characters;
    BulletController* bullets;
    MonsterController* monsters;
    PropController* props;

    MonsterThread* monsters2;

    HitMonsterThread* hitMonster;
    HitBarrierThread* hitBarrier;
    HitCharacterThread* hitCharacter;
    PickThingsThread* pickThings;

    QThread* characterBulletsThread;

    Map* map;

    QSqlDatabase db;

    int gameTime;

    bool isRunning;

};

#endif // GAMEWIDGET_H
