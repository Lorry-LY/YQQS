#ifndef MONSTERBULLET_H
#define MONSTERBULLET_H

#include "model/bullet.h"

class MonsterBullet : public Bullet
{
public:
    MonsterBullet();

    MonsterBullet(QWidget *parent,unsigned ATK,float shootAngle,float speed,QString URL,QPoint point,int offset);

    void hitCharacter();
};

#endif // MONSTERBULLET_H
