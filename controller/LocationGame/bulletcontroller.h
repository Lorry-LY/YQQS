#ifndef BULLETCONTROLLER_H
#define BULLETCONTROLLER_H

#include <QWidget>
#include <QPropertyAnimation>

#include "model/bullet.h"

class BulletController : public QWidget
{
    Q_OBJECT
public:
    explicit BulletController(QWidget *parent = nullptr);

    void updateBullets();

    void removeBullet(Bullet* bullet);

    void addCharacterBullet(Bullet* bullet);

    void judge(Bullet* bullet);

signals:
    void bulletMove(Bullet* bullet);
    void addAnimation(QPropertyAnimation* pPosAnimation1);

private:
    void deleteBullets();

signals:


public:
    QVector<Bullet*> monsterBullets;
    QVector<Bullet*> characterBullets;

};

#endif // BULLETCONTROLLER_H
