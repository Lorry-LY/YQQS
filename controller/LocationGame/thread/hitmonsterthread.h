#ifndef HITMONSTERTHREAD_H
#define HITMONSTERTHREAD_H

#include <QObject>
#include <QThread>
#include <QPair>
#include "model/bullet.h"
#include "model/monstercharacter.h"
#include "model/map.h"

class HitMonsterThread : public QObject
{
	Q_OBJECT
public:
	explicit HitMonsterThread(Map* map, QObject* parent = nullptr);

	void judge(Bullet* bullet);

	void print();

signals:
	void hit(Bullet* bullet, MonsterCharacter* monster);
	void finished();

private:
	double distance(QPoint x1, QPoint x2);

private:
	Map* map;

};

#endif // HITMONSTERTHREAD_H
