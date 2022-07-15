#include "pickthingsthread.h"
#include <QtMath>

PickThingsThread::PickThingsThread(Map *map, QObject *parent) : QObject(parent)
{
    this->map =map;
}

void PickThingsThread::judge(Character *character)
{
    QList<MagicPoint*> magics = ((Map*)(character->parentWidget()))->getCurrentHouse(character)->findChildren<MagicPoint*>();
    while(!magics.isEmpty())
    {
        MagicPoint* magic = magics.front();
        magics.pop_front();
        if(!magic->getPicked() && distance(magic->getPoint()+magic->parentWidget()->pos(),character->getPoint())<300)
        {
            emit pickMagic(character,magic);
        }
    }
    QList<Coin*> coins = ((Map*)(character->parentWidget()))->getCurrentHouse(character)->findChildren<Coin*>();
    while(!coins.isEmpty())
    {
        Coin* coin = coins.front();
        coins.pop_front();
        if(!coin->getPicked() && distance(coin->getPoint()+coin->parentWidget()->pos(),character->getPoint())<300)
        {
            emit pickCoin(character,coin);
        }
    }
}

double PickThingsThread::distance(QPoint x1, QPoint x2)
{
    return qSqrt((x1.x() - x2.x()) * (x1.x() - x2.x()) + (x1.y() - x2.y()) * (x1.y() - x2.y()));
}
