#ifndef MONSTERCONTROLLER_H
#define MONSTERCONTROLLER_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "model/monstercharacter.h"
#include "model/map.h"
#include "model/monsterbullet.h"
#include "controller/mediaController/mediacontroller.h"

class MonsterController : public QWidget
{
    Q_OBJECT
public:
    explicit MonsterController(QWidget *parent = nullptr);

    void dealMonsterDead(Character* monster);
    void createCharacter(MonsterCharacter* monsterCharacter);
    void creatMonster(MonsterCharacter *monsterCharacter);
    void init();
    void shoot(MonsterCharacter *monsterCharacter,float angle);
    void changeMonsterWeaponAngle();
    //    void dealShoot(MonsterCharacter *monsterCharacter);
    //    void changeWeaponAngle(MonsterCharacter *monsterCharacter);
    //    void changeWeaponAngle(MonsterCharacter *monsterCharacter,float angle);
    bool noWall(QPoint start,QPoint end,float angle);
    bool isActive(MonsterCharacter *monsterCharacter);
    void moveToCharacter(MonsterCharacter *monsterCharacter,float angle);
    void moveRandom();
    void creatMonster();

    void judgeMove(Character *character, QPoint point ,QPoint movePoint);
    void judgeRepel(Character *character, QPoint point);

    void controller(SmallMap* smallMap);
    void uncontroller(SmallMap* smallMap);
    void changeWeaponAngle();

    QVector<MonsterCharacter*> monsters;
    MediaController *media;
private:
    void moved(MonsterCharacter *monsterCharacter);
    void shooted(MonsterCharacter *monsterCharacter);

    void changeWeaponAngle(MonsterCharacter *monsterCharacter);
    void changeWeaponAngle(MonsterCharacter *monster, float angle);
    bool isSeeCharacter(MonsterCharacter *monster,Character* character,QPointF& angle);
    void moveToCharacter(MonsterCharacter *monster, QPointF& angle);
    double distance(QPoint x1, QPoint x2);

signals:
    void changeView(QPoint point);
    void addBulletMonster(MonsterBullet *bullet);

private:
    QSqlDatabase db;
    SmallMap* smallMap;

};

#endif // MONSTERCONTROLLER_H
