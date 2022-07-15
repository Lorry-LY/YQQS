#include "monsterbullet.h"

MonsterBullet::MonsterBullet()
{

}

MonsterBullet::MonsterBullet(QWidget* parent, unsigned ATK, float shootAngle, float speed, QString URL, QPoint point, int offset) : Bullet(parent,ATK,shootAngle,speed,URL,point,offset)
{

}

void MonsterBullet::hitCharacter()
{
    hitMonster();
}
