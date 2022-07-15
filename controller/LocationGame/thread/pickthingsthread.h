#ifndef PickThingsThread_H
#define PickThingsThread_H

#include <QObject>
#include "model/map.h"
#include "model/magicpoint.h"
#include "model/coin.h"
#include "model/character.h"

class PickThingsThread : public QObject
{
    Q_OBJECT
public:
    explicit PickThingsThread(Map* map,QObject *parent = nullptr);

    void judge(Character* character);

signals:
    void pickMagic(Character* character,MagicPoint* magic);
    void pickCoin(Character* character,Coin* coin);

private:
    double distance(QPoint x1, QPoint x2);

private:
    Map* map;
};

#endif // PickThingsThread_H
