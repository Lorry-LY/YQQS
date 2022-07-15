#ifndef SMALLMAP_H
#define SMALLMAP_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include "element.h"
#include "model/monstercharacter.h"
#include "model/box.h"
#include "tools/myanimation.h"
#include <QSqlDatabase>
#include <QSqlQuery>

class SmallMap : public QWidget
{
    Q_OBJECT
public:
    explicit SmallMap(QWidget *parent = nullptr);

public:
    void init();

    void update(int type);

    void stopGame();
    void continueGame();

    void createMonster();

    bool isEnter(QPoint point);

    QPoint getPoint(QPoint point);

    Element::_type getState_isMove(QPoint point);
    Element::_type getState_isShoot(QPoint point);

    void buildRoad(int way);

    void closeDoor();
    void openDoor();

private:
    void buildWall();
    void buildFlat();
    QPoint createPoint();
    void buildShop();
    void buildPortal();
    void buildMagic();
    void buildBarriers();
    void Barriers0();
    void Barriers1();
    void Barriers2();
    void Barriers3();
    void Barriers4();
    void buildBox();
    int tag = 1;
    void onTimeOut();
    QSqlDatabase db;

signals:

public:
    Element* elements[45][45];   //小元素

    enum _type{
        NONE = 0,
        NORMAL = 1,
        START = 2,
        END = 3,
        SHOP = 4,
        FIGHT = 5,
        MAGIC = 6
    }type;

    int width;
    int height;


};

#endif // SMALLMAP_H
