#ifndef CHARACTER_H
#define CHARACTER_H

#include "model/weapon.h"
#include "model/bodylabel.h"
#include <QWidget>
#include <QLabel>

class Character : public QWidget
{
    Q_OBJECT
public:
    explicit Character(QWidget *parent = nullptr);
    Character(QWidget *parent,QString name,int maxHP,int maxMP,int maxShield,QString bodyImage,Weapon* weapon,float speed);

    QPoint getPoint();
    void setPoint(QPoint point);

    Weapon *getWeapon();
    void setWeapon(Weapon* weapon);

    unsigned getHP();
    void setHP(int HP);
    void changeHP(int HP);

    unsigned getMP();
    void setMP(int MP);
    void changeMP(int MP);

    unsigned getShield();
    void setShield(int Shield);
    void changeShield(int Shield);

    unsigned getCoin();
    void setCoin(int Coin);
    void changeCoin(int Coin);

    void hitted(int ATK,float repelAngle);

    void changeMoveWay(int way,int value);

    void moveStep(QPoint point);

    void moveRepel(QPoint point);

    void setAlive(bool alive);
    bool isAlive();

    void stopGame();
    void continueGame();

    void death();

signals:
    void sendPosition(Character *character,QPoint point,QPoint movePoint);
    void sendRepelPosition(Character *character,QPoint point);
    void dead(Character* character);
    void changedHP(int,int);
    void changedMP(int,int);
    void changedShied(int,int);
    void changedCoin(int);

public:
    unsigned ID;     //编号
    QString name;   //角色名字
    unsigned HP;     //生命值
    unsigned MP;     //能量
    unsigned shield;    //护盾
    unsigned MaxHP;
    unsigned MaxMP;
    unsigned MaxShield;
    int coin;
    enum _state{         //状态
        STOP = 1,
        MOVE = 2,
        HELP = 3,
        DEAD = 4
    }state;
    QWidget headWidget;
    QString bodyURL;   //身体图片
    BodyLabel bodyImage;   //脚步图片
    Weapon* weapon;      //武器
    float speed;        //速度

    float moveWay[4];

    bool skillEnable;
    bool isSkillTime;

    int recoverTime;

    bool alive;

    bool gameStop;

};

#endif // CHARACTER_H
