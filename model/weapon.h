#ifndef WEAPON_H
#define WEAPON_H

#include <QWidget>
#include <QLabel>
#include "model/bullet.h"

class Weapon : public QLabel
{
    Q_OBJECT
public:
    explicit Weapon(QWidget *parent = nullptr);
    Weapon(QWidget *parent,QString name,int ATK,int MP,float speed,float crit,int type,QString weaponImage, QString bulletImage,float bulletSpeed);

    QPoint getPoint();
    void setPoint(QPoint point);

    float getAngle();
    void setAngle(float angle);
    void setAngle();

    unsigned getSpell();

//    void pick(Character* character);
//    void abandon(Character* character);

    void setEnable(bool enable);
    bool getEnable();


protected:
    void paintEvent(QPaintEvent *e);

signals:

public:
    QString name;
    unsigned ATK;   //攻击力
    unsigned spell;     //消耗的能量
    float crit;     //暴击率
    float speed;    //攻击速度
//    enum _state{     //状态
//        ABANDON = 1,
//        HANDLE = 2
//    }state;
    enum _type{
        GUN = 1,
        WAND = 2
    }type;

    bool enable;
    float angle;
    QString bulletImage;        //子弹照片
    float bulletSpeed;

    Q_ENUM(_type)
//    Q_ENUM(_state)


};

#endif // WEAPON_H
