#ifndef MONSTERCHARACTER_H
#define MONSTERCHARACTER_H

#include "model/character.h"

class MonsterCharacter : public Character
{
    Q_OBJECT
public:
    explicit MonsterCharacter(QWidget* parent = nullptr);
    bool tagActive;
    MonsterCharacter(QWidget* parent, QString name, int maxHP, int maxMP,int maxShield, int coin, QString bodyImage, Weapon* weapon, float speed);
    MonsterCharacter(QWidget* parent, QString name, int maxHP, int maxMP, int maxShield, QString bodyImage, Weapon* weapon, float speed);

    bool isActive();
    void setActive(bool active);

    void setMoveWay(QPointF point);

    void stopGame();
    void continueGame();

signals:
    void needMove(MonsterCharacter* monster);
    void needShoot(MonsterCharacter* monster);

};

#endif // MONSTERCHARACTER_H
