#include "hitcharacterthread.h"
#include <QDebug>
#include <QtMath>

HitCharacterThread::HitCharacterThread(Map* map, QObject *parent) : QObject(parent)
{
    this->map = map;
}

void HitCharacterThread::judge(Bullet* bullet)
{
	//qDebug() << bullets.size();
    QList<Character*> characters = map->findChildren<Character*>("",Qt::FindDirectChildrenOnly);
	while (!characters.isEmpty())
	{
        Character* character = characters.front();
		characters.pop_front();
        if (character->isAlive()&&distance(character->getPoint(), bullet->getPoint()) <= 45)
		{
			emit hit(bullet, character);
			return;
		}
	}
}

double HitCharacterThread::distance(QPoint x1, QPoint x2)
{
    return sqrt((x1.x() - x2.x()) * (x1.x() - x2.x()) + (x1.y() - x2.y()) * (x1.y() - x2.y()));
}

void HitCharacterThread::print()
{
    qDebug() << QThread::currentThreadId();
}

