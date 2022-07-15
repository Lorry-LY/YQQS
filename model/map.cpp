#include "map.h"
#include <QTime>
#include <QGridLayout>
#include <QPair>

Map::Map(QWidget *parent) : QWidget(parent)
{
    this->number = 1;
    //    QGridLayout *layout = new QGridLayout(this);
    //    layout->setSpacing(0);
    //    layout->setHorizontalSpacing(0);
    //    layout->setVerticalSpacing(0);
    //    layout->setContentsMargins(0,0,0,0);
    //setGeometry(0,0,1800,1800);
    setFixedSize(5400,5400);
    resize(5400,5400);
    //setStyleSheet("QWidget{background-color: rgb(170, 255, 255);}");
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            smallMap[i][j] = new SmallMap(this);
            //            layout->addWidget(smallMap[i][j],i,j);
            smallMap[i][j]->setGeometry(j*1350,i*1350,1350,1350);
            smallMap[i][j]->setObjectName(QString("(%1,%2)").arg(i).arg(j));

        }
    }

    //    房间1
    QVector<QPair<QPoint, SmallMap::_type>>seed;
    seed.push_back({QPoint(1,0),SmallMap::START});
    seed.push_back({QPoint(1,1),SmallMap::FIGHT});
    seed.push_back({QPoint(0,1),SmallMap::MAGIC});
    seed.push_back({QPoint(2,1),SmallMap::SHOP});
    seed.push_back({QPoint(1,2),SmallMap::FIGHT});
    seed.push_back({QPoint(1,3),SmallMap::FIGHT});
    seed.push_back({QPoint(2,3),SmallMap::END});

    seeds.push_back(seed);

    //    房间2
    QVector<QPair<QPoint, SmallMap::_type>>seed2;
    seed2.push_back({QPoint(3,1),SmallMap::START});
    seed2.push_back({QPoint(2,1),SmallMap::FIGHT});
    seed2.push_back({QPoint(2,2),SmallMap::MAGIC});
    seed2.push_back({QPoint(2,0),SmallMap::FIGHT});
    seed2.push_back({QPoint(1,0),SmallMap::FIGHT});
    seed2.push_back({QPoint(0,0),SmallMap::END});
    seeds.push_back(seed2);

    //    房间3
    QVector<QPair<QPoint, SmallMap::_type>>seed3;
    seed3.push_back({QPoint(0,1),SmallMap::START});
    seed3.push_back({QPoint(1,1),SmallMap::FIGHT});
    seed3.push_back({QPoint(1,2),SmallMap::SHOP});
    seed3.push_back({QPoint(1,0),SmallMap::FIGHT});
    seed3.push_back({QPoint(2,0),SmallMap::FIGHT});
    seed3.push_back({QPoint(3,0),SmallMap::END});
    seeds.push_back(seed3);

    //    房间4
    QVector<QPair<QPoint, SmallMap::_type>>seed4;
    seed4.push_back({QPoint(3,1),SmallMap::START});
    seed4.push_back({QPoint(2,1),SmallMap::FIGHT});
    seed4.push_back({QPoint(2,2),SmallMap::MAGIC});
    seed4.push_back({QPoint(2,0),SmallMap::FIGHT});
    seed4.push_back({QPoint(1,0),SmallMap::END});
    seed4.push_back({QPoint(1,1),SmallMap::SHOP});
    seeds.push_back(seed4);

    //    房间5
    QVector<QPair<QPoint, SmallMap::_type>>seed5;
    seed5.push_back({QPoint(0,2),SmallMap::START});
    seed5.push_back({QPoint(1,2),SmallMap::FIGHT});
    seed5.push_back({QPoint(1,3),SmallMap::SHOP});
    seed5.push_back({QPoint(1,1),SmallMap::FIGHT});
    seed5.push_back({QPoint(1,0),SmallMap::FIGHT});
    seed5.push_back({QPoint(2,1),SmallMap::END});
    seed5.push_back({QPoint(2,2),SmallMap::MAGIC});
    seeds.push_back(seed5);

}

Map::Map(QWidget *parent, int number, int type)
{
    this->setParent(parent);
    this->number = number;
    this->type = Map::_type(type);
}

void Map::startNew(Character* character)
{
    number = 1;
    init();
    updateMap();
    character->setParent(this);
    QPoint point(currentSeed[0].first);
    point = QPoint(point.y()*1350,point.x()*1350);
    int dx,dy;
    while ((dx=qrand()%720)<600);
    while ((dy=qrand()%720)<600);
    point = point + QPoint(dx,dy);
    character->move(point);
    emit finished(character);
}

void Map::continueNew(Character *character)
{
    number += 1;
    init();
    updateMap();
    QPoint point(currentSeed[0].first);
    point = QPoint(point.y()*1350,point.x()*1350);
    int dx,dy;
    while ((dx=qrand()%720)<600);
    while ((dy=qrand()%720)<600);
    point = point + QPoint(dx,dy);
    character->move(point);
    emit finished(character);
}

void Map::continueGame(int number, Character *character)
{
    this->number=number;
    init();
    updateMap();
    QPoint point(currentSeed[0].first);
    point = QPoint(point.y()*1350,point.x()*1350);
    int dx,dy;
    while ((dx=qrand()%720)<600);
    while ((dy=qrand()%720)<600);
    point = point + QPoint(dx,dy);
    character->move(point);
    emit finished(character);
}

void Map::updateMap()
{
    currentSeed = buildMapList();
    for (int i = 0;i<currentSeed.size();i++) {
        smallMap[currentSeed[i].first.x()][currentSeed[i].first.y()]->update(currentSeed[i].second);
    }
    for(int i=0;i<currentSeed.size();i++)
    {
        QPoint d[4] = {{1,0},{0,1},{0,-1},{-1,0}};
        for(int j=0;j<currentSeed.size();j++)
        {
            if(i==j)continue;
            for(int k=0;k<4;k++)
            {
                if(currentSeed[i].first+d[k] == currentSeed[j].first)
                {
                    smallMap[currentSeed[i].first.x()][currentSeed[i].first.y()]->buildRoad(k);
                }
            }
        }
    }
    int num = 0;
    for (int i=0;i<currentSeed.size();i++)
    {
        if(currentSeed[i].second == SmallMap::_type::FIGHT)
        {
            num+=4+number;
            for (int j=0;j<num;j++)
            {
                smallMap[currentSeed[i].first.x()][currentSeed[i].first.y()]->createMonster();
            }
        }
    }

    emit fightting(smallMap[currentSeed[0].first.x()][currentSeed[0].first.y()]);
    isFighted = false;
}

void Map::stopGame()
{
    QList<Character*> characters = findChildren<Character*>("",Qt::FindDirectChildrenOnly);
    while(!characters.isEmpty())
    {
        Character* character = characters.front();
        characters.pop_front();
        character->stopGame();
    }
    QList<Bullet*> bullets = findChildren<Bullet*>("",Qt::FindDirectChildrenOnly);
    while(!bullets.isEmpty())
    {
        Bullet* bullet = bullets.front();
        bullets.pop_front();
        bullet->stopGame();
    }
    QList<MonsterBullet*> monsterBullets = findChildren<MonsterBullet*>("",Qt::FindDirectChildrenOnly);
    while(!monsterBullets.isEmpty())
    {
        MonsterBullet* monsterBullet = monsterBullets.front();
        monsterBullets.pop_front();
        monsterBullet->stopGame();
    }
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            smallMap[i][j]->stopGame();
        }
    }
}

void Map::continueGame()
{
    QList<Character*> characters = findChildren<Character*>("",Qt::FindDirectChildrenOnly);
    while(!characters.isEmpty())
    {
        Character* character = characters.front();
        characters.pop_front();
        character->continueGame();
    }
    QList<Bullet*> bullets = findChildren<Bullet*>("",Qt::FindDirectChildrenOnly);
    while(!bullets.isEmpty())
    {
        Bullet* bullet = bullets.front();
        bullets.pop_front();
        bullet->continueGame();
    }
    QList<MonsterBullet*> monsterBullets = findChildren<MonsterBullet*>("",Qt::FindDirectChildrenOnly);
    while(!monsterBullets.isEmpty())
    {
        MonsterBullet* monsterBullet = monsterBullets.front();
        monsterBullets.pop_front();
        monsterBullet->continueGame();
    }
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            smallMap[i][j]->continueGame();
        }
    }
}

void Map::judge(Character *character)
{
    if(isFighted)   //开门
    {
        bool isOpenDoor = true;
        QList<MonsterCharacter*> monsters = getCurrentHouse(character)->findChildren<MonsterCharacter*>("",Qt::FindDirectChildrenOnly);
        while(!monsters.isEmpty())
        {
            MonsterCharacter* monster = monsters.front();
            monsters.pop_front();
            if(monster->isAlive())
            {
                isOpenDoor = false;
                monsters.clear();
                break;
            }
        }
        if(isOpenDoor)
        {
            emit unfightting(getCurrentHouse(character));
            getCurrentHouse(character)->openDoor();
            isFighted = false;
        }
    }
    else            //关门
    {
        bool isCloseDoor = false;
        if(getCurrentHouse(character)->type == SmallMap::_type::FIGHT)
        {
            if(getCurrentHouse(character)->isEnter(QPoint(character->getPoint().x()%1350,character->getPoint().y()%1350)))
            {
                QList<MonsterCharacter*> monsters = getCurrentHouse(character)->findChildren<MonsterCharacter*>("",Qt::FindDirectChildrenOnly);
                while(!monsters.isEmpty())
                {
                    MonsterCharacter* monster = monsters.front();
                    monsters.pop_front();
                    if(monster->isAlive())
                    {
                        isCloseDoor = true;
                        monsters.clear();

                        break;
                    }
                }
            }
        }
        if(isCloseDoor)
        {
            emit fightting(getCurrentHouse(character));
            getCurrentHouse(character)->closeDoor();
            isFighted = true;
        }
    }
}

SmallMap *Map::getCurrentHouse(Character *character)
{
    return smallMap[character->getPoint().y()/1350][character->getPoint().x()/1350];
}

QPoint Map::getPoint(QPoint point)
{
    int x = point.y()/1350;
    int y = point.x()/1350;
    return smallMap[x][y]->getPoint(QPoint(point.x()%1350,point.y()%1350));
}

Element::_type Map::getState_isMove(QPoint point)
{

    int x = point.y()/1350;
    int y = point.x()/1350;
    if(x<45&&y<45){
        return smallMap[x][y]->getState_isMove(QPoint(point.x()%1350,point.y()%1350));
    }
    else return Element::_type::NONE;
    //    qDebug()<<x<<y;

}

Element::_type Map::getState_isShoot(QPoint point)
{
    //qDebug()<<point;
    int y = point.y()/1350;
    int x = point.x()/1350;
    //qDebug() << y << x;
    //qDebug() << point.x() % 1500 << point.y() % 1500;
    return smallMap[y][x]->getState_isShoot(QPoint(point.x()%1350,point.y()%1350));
}

void Map::init()
{
    QList<Bullet*> bullets = findChildren<Bullet*>("",Qt::FindDirectChildrenOnly);
    while(!bullets.isEmpty())
    {
        Bullet* bullet = bullets.front();
        bullets.pop_front();
        delete bullet;
    }
    QList<MonsterBullet*> monsterBullets = findChildren<MonsterBullet*>("",Qt::FindDirectChildrenOnly);
    while(!monsterBullets.isEmpty())
    {
        MonsterBullet* monsterBullet = monsterBullets.front();
        monsterBullets.pop_front();
        delete monsterBullet;
    }
    QList<Weapon*> weapons = findChildren<Weapon*>("",Qt::FindDirectChildrenOnly);
    while(!weapons.isEmpty())
    {
        Weapon* weapon = weapons.front();
        weapons.pop_front();
        delete weapon;
    }
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            smallMap[i][j]->init();
        }
    }
}

QVector<QPair<QPoint, SmallMap::_type>> Map::buildMapList()
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    int number = qrand()%seeds.size();
//    qDebug()<<number;
    return seeds[number];
}



