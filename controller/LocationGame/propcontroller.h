#ifndef PROPCONTROLLER_H
#define PROPCONTROLLER_H

#include <QWidget>
#include "model/magicpoint.h"
#include "model/coin.h"
#include "model/box.h"
#include "model/weapon.h"

class PropController : public QWidget
{
    Q_OBJECT
public:
    explicit PropController(QWidget *parent = nullptr);

signals:

public:
    QVector<MagicPoint*> MPs;
    QVector<Coin*> coins;
    QVector<Box*> HPBoxs;
    QVector<Box*> MPBoxs;
    QVector<Weapon*> weapons;

};

#endif // PROPCONTROLLER_H
