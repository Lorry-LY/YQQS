#ifndef HITBARRIERTHREAD_H
#define HITBARRIERTHREAD_H

#include <QObject>
#include "model/map.h"
#include "model/bullet.h"

class HitBarrierThread : public QObject
{
    Q_OBJECT
public:
    explicit HitBarrierThread(Map* map , QObject *parent = nullptr);

    void judge(Bullet* bullet);

signals:
    void hitBarrier(Bullet* bullet);

private:
    Map* map;

};

#endif // HITBARRIERTHREAD_H
