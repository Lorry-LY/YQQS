#include "hitmonsterthread.h"
#include <QDebug>
#include <QtMath>

HitMonsterThread::HitMonsterThread(Map* map, QObject* parent) : QObject(parent)
{
    this->map = map;
}

void HitMonsterThread::judge(Bullet* bullet)
{
	//qDebug() << bullets.size();
    QList<MonsterCharacter*> characters = map->findChildren<MonsterCharacter*>();
	while (!characters.isEmpty())
	{
		MonsterCharacter* character = characters.front();
		characters.pop_front();
        if (character->isAlive()&&distance(character->getPoint()+character->parentWidget()->pos(), bullet->getPoint()) <= 45)
		{
			emit hit(bullet, character);
			return;
		}
	}
}

double HitMonsterThread::distance(QPoint x1, QPoint x2)
{
	return sqrt((x1.x() - x2.x()) * (x1.x() - x2.x()) + (x1.y() - x2.y()) * (x1.y() - x2.y()));
}


void HitMonsterThread::print()
{
    qDebug() << QThread::currentThreadId();
}
