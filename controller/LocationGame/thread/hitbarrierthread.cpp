#include "hitbarrierthread.h"

HitBarrierThread::HitBarrierThread(Map* map , QObject *parent) : QObject(parent)
{
	this->map = map;
}

void HitBarrierThread::judge(Bullet* bullet)
{
	//qDebug() << bullet->getPoint();
	//qDebug() << bullet->geometry();
    if (map->getState_isShoot(bullet->getPoint()) == Element::_type::BARRIER)
	{
		emit hitBarrier(bullet);
	}
}
