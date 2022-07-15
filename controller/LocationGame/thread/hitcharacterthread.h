#ifndef HitCharacterThreadTHREAD_H
#define HitCharacterThreadTHREAD_H

#include "model/map.h"
#include "model/monsterbullet.h"
#include "model/monstercharacter.h"

#include <QObject>
#include <QThread>

class HitCharacterThread : public QObject
{
	Q_OBJECT
public:
	explicit HitCharacterThread(Map* map, QObject* parent = nullptr);

    void judge(Bullet* bullet);

	void print();

signals:
    void hit(Bullet* bullet, Character* character);
	void finished();

private:
	double distance(QPoint x1, QPoint x2);

private:
	Map* map;

};

#endif // HitCharacterThreadTHREAD_H
