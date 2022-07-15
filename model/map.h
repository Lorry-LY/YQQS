#ifndef MAP_H
#define MAP_H

#include <QWidget>
#include <QLabel>
#include <QMatrix4x4>
#include "smallmap.h"
#include "model/monstercharacter.h"
#include "model/monsterbullet.h"

class Map : public QWidget
{
    Q_OBJECT
public:
    explicit Map(QWidget *parent = nullptr);
    Map(QWidget *parent,int number,int type);

    void startNew(Character* character);
    void continueNew(Character* character);
    void continueGame(int number,Character* character);

    void updateMap();
    void stopGame();
    void continueGame();

    void judge(Character* character);

    SmallMap* getCurrentHouse(Character *character);

    QPoint getPoint(QPoint point);

    Element::_type getState_isMove(QPoint point);
    Element::_type getState_isShoot(QPoint point);

    void init();

private:
    QVector<QPair<QPoint,SmallMap::_type>> buildMapList();


signals:
    void fightting(SmallMap* smallMap);
    void unfightting(SmallMap* smallMap);
    void finished(Character* character);

public:
    int number;     //关卡数
    enum _type{
        ICE = 1,
        STONE = 2,
        JUNGLE = 3
    }type;

    SmallMap* smallMap[4][4];    //地图块

    QVector<QVector<QPair<QPoint, SmallMap::_type>>> seeds;
    QVector<QPair<QPoint, SmallMap::_type>> currentSeed;

private:
    bool isFighted;
};

#endif // MAP_H
