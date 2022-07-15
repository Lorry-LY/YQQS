#ifndef MONSTERTHREAD_H
#define MONSTERTHREAD_H

#include <QObject>
#include "model/character.h"


class MonsterThread : public QObject
{
    Q_OBJECT
public:
    explicit MonsterThread(QObject *parent = nullptr);

    void updateState(QVector<Character*> characters,QVector<Character*> monsters);

signals:
    void move(Character* montser,char moveWay);
    void shoot(Character* montser,float Angle);
    void changeWeaponAngle(Character* montser,float Angle);

};

#endif // MONSTERTHREAD_H
