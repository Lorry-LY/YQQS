#include "monstercontroller.h"
#include "model/coin.h"
#include "model/magicpoint.h"
#include <QDebug>
#include <QtMath>
#include <QPainter>
#include <QTimer>

MonsterController::MonsterController(QWidget *parent) : QWidget(parent)
{
    media = this->parentWidget()->parentWidget()->parentWidget()->findChild<MediaController*>("media");
}

void MonsterController::dealMonsterDead(Character *monster)
{
    int magicNumber = monster->getMP() / 10;
    int coinNumber = monster->getCoin();
//    qDebug()<<magicNumber<<coinNumber;
    for(int i =0;i<magicNumber;i++)
    {
        MagicPoint* magic = new MagicPoint(monster->parentWidget(),monster->getPoint());
        connect(magic,&MagicPoint::pick,this,[=](Character* character){
            QMediaPlayer *shot = new QMediaPlayer(this);
            shot->setMedia(media->findChild<QMediaPlayer*>("得到")->media());
            shot->setVolume(media->findChild<QMediaPlayer*>("得到")->volume());
            shot->play();
            connect(shot,&QMediaPlayer::stateChanged,shot,&QMediaPlayer::deleteLater);
            character->changeMP(magic->value);
            delete  magic;
        });
        magic->show();
    }
    for (int i=0;i<coinNumber;i++)
    {
        Coin* coin = new Coin(monster->parentWidget(),monster->getPoint());
        connect(coin,&Coin::pick,this,[=](Character* character){
            QMediaPlayer *shot = new QMediaPlayer(this);
            shot->setMedia(media->findChild<QMediaPlayer*>("得到")->media());
            shot->setVolume(media->findChild<QMediaPlayer*>("得到")->volume());
            shot->play();
            connect(shot,&QMediaPlayer::stateChanged,shot,&QMediaPlayer::deleteLater);
           character->changeCoin(coin->value);
            delete  coin;
        });
        coin->show();
    }

}

void MonsterController::init()
{
    QPoint characterP=parentWidget()->findChild<Map*>("map")->findChild<Character*>("hero")->getPoint();
    foreach (MonsterCharacter *m,monsters){
        QPoint t=m->getPoint()+QPoint(m->parentWidget()->geometry().x(),m->parentWidget()->geometry().y());
        QPoint angleP=QPoint(characterP.x()-t.x(),characterP.y()-t.y());
        float angle=qAtan2(angleP.y(),angleP.x()) * 180 / M_PI;
        moveToCharacter(m,angle);
        //        QTimer* updateTimer2 = new QTimer(this);
        //        updateTimer2->setInterval(1000);
        //        connect(updateTimer2, &QTimer::timeout, this, &MonsterController::dealShoot);
        //        updateTimer2->setObjectName("updateTimer2");
        //        updateTimer2->start();
        shoot(m,angle);
    }



    //    if (qrand() % 100 > 50 && noWall(monsterP,characterP,angle)){
    //        moveToCharacter(monsterCharacter,angle);
    //    }
    //    else{
    //        moveRandom();
    //    }
    //    shoot();
}

void MonsterController::shoot(MonsterCharacter *monsterCharacter,float angle)
{
    if (monsterCharacter->getWeapon()->getEnable() == false)return;
    monsterCharacter->getWeapon()->setEnable(false);
    QTimer::singleShot(monsterCharacter->getWeapon()->speed, this, [=]() {monsterCharacter->getWeapon()->setEnable(true); });
    monsterCharacter->changeMP(monsterCharacter->getWeapon()->getSpell());
    float speed = 50;
    MonsterBullet* bullet = new MonsterBullet((QWidget*)parentWidget()->findChild<Map*>("map"), monsterCharacter->getWeapon()->ATK,
                                              angle, speed, monsterCharacter->getWeapon()->bulletImage, monsterCharacter->getPoint()+
                                              QPoint(monsterCharacter->parentWidget()->geometry().x(),monsterCharacter->parentWidget()->geometry().y()),
                                              monsterCharacter->getWeapon()->width() / 2);

    QMediaPlayer *shot = new QMediaPlayer(this);
    shot->setMedia(media->findChild<QMediaPlayer*>("怪物枪声")->media());
    shot->setVolume(media->findChild<QMediaPlayer*>("怪物枪声")->volume());
    shot->play();
    connect(shot,&QMediaPlayer::stateChanged,shot,&QMediaPlayer::deleteLater);
    emit addBulletMonster(bullet);
}

void MonsterController::changeMonsterWeaponAngle(){
    QPoint characterP=parentWidget()->findChild<Map*>("map")->findChild<Character*>("hero")->getPoint();
    foreach (MonsterCharacter *m,monsters){
        QPoint t=m->getPoint()+QPoint(m->parentWidget()->geometry().x(),m->parentWidget()->geometry().y());
        QPoint angleP=QPoint(characterP.x()-t.x(),characterP.y()-t.y());
        float angle=qAtan2(angleP.y(),angleP.x()) * 180 / M_PI;
        m->weapon->setAngle(angle);
    }
}

bool isActive(MonsterCharacter *monsterCharacter)
{
    if (monsterCharacter->tagActive){
        return true;
    }
    return false;
}

void MonsterController::moveToCharacter(MonsterCharacter *monsterCharacter,float angle){
    QPoint p=monsterCharacter->getPoint();
    p.setX(monsterCharacter->x()+4*cos(angle*M_PI/180));
    p.setY(monsterCharacter->y()+4*sin(angle*M_PI/180));
    monsterCharacter->move(p);
}

void MonsterController::creatMonster()
{
    QSqlQuery query = QSqlQuery(db);
    query.exec(QString("select c.'名称',c.'生命值',c.'能量',c.'护盾',c.'移动速度',c.'身体图片'"
                       ",w.'名称',w.'威力',w.'能量消耗',w.'攻击速度',w.'暴击率',w.'类型',w.URL,w.'子弹图片',w.'子弹速度' "
                       "from '角色' c,'武器' w "
                       "where c.'初始武器ID' = w.ID and c.'名称' = '法师怪物'"));
    query.next();
    QString name = query.value("名称").toString();
    int HP = query.value("生命值").toInt();
    int MP = query.value("能量").toInt();
    int shield = query.value("护盾").toInt();
    float speed = query.value("移动速度").toDouble();
    QString bodyURL = query.value("身体图片").toString();
    QString weaponName = query.value("名称").toString();
    int ATK = query.value("威力").toInt();
    int dMP = query.value("能量消耗").toInt();
    float crit = query.value("暴击率").toFloat();
    float Aspeed = query.value("攻击速度").toFloat();
    int type = query.value("类型").toInt();
    QString weaponURL = query.value("URL").toString();
    QString bulletURL = query.value("子弹图片").toString();
    float bulletSpeed = query.value("子弹速度").toFloat();
    Weapon* weapon = new Weapon(nullptr, weaponName, ATK, dMP, Aspeed, crit, type, weaponURL, bulletURL, bulletSpeed);
    MonsterCharacter* monster = new MonsterCharacter(parentWidget()->findChild<Map*>("map")->smallMap[parentWidget()->findChild<Map*>("map")->seeds[0][1].first.x()][parentWidget()->findChild<Map*>("map")->seeds[0][1].first.y()], name, HP, MP, shield, bodyURL, weapon, speed);
    monster->move(600%1350, 800%1350);
    weapon->setParent(monster);
    monsters.push_back(monster);


    query.exec(QString("select c.'名称',c.'生命值',c.'能量',c.'护盾',c.'移动速度',c.'身体图片'"
                       ",w.'名称',w.'威力',w.'能量消耗',w.'攻击速度',w.'暴击率',w.'类型',w.URL,w.'子弹图片',w.'子弹速度' "
                       "from '角色' c,'武器' w "
                       "where c.'初始武器ID' = w.ID and c.'名称' = '野猪怪物'"));
    query.next();
    name = query.value("名称").toString();
    HP = query.value("生命值").toInt();
    MP = query.value("能量").toInt();
    shield = query.value("护盾").toInt();
    speed = query.value("移动速度").toDouble();
    bodyURL = query.value("身体图片").toString();
    weaponName = query.value("名称").toString();
    ATK = query.value("威力").toInt();
    dMP = query.value("能量消耗").toInt();
    crit = query.value("暴击率").toFloat();
    Aspeed = query.value("攻击速度").toFloat();
    type = query.value("类型").toInt();
    weaponURL = query.value("URL").toString();
    bulletURL = query.value("子弹图片").toString();
    bulletSpeed = query.value("子弹速度").toFloat();
    weapon = new Weapon(nullptr, weaponName, ATK, dMP, Aspeed, crit, type, weaponURL, bulletURL, bulletSpeed);
    monster = new MonsterCharacter(parentWidget()->findChild<Map*>("map")->smallMap[parentWidget()->findChild<Map*>("map")->seeds[0][1].first.x()][parentWidget()->findChild<Map*>("map")->seeds[0][1].first.y()], name, HP, MP, shield, bodyURL, weapon, speed);
    monster->move(1000%1350, 300%1350);
    weapon->setParent(monster);
    monsters.push_back(monster);

}

void MonsterController::controller(SmallMap* smallMap)
{
    this->smallMap = smallMap;
    QList<MonsterCharacter*> monsters = smallMap->findChildren<MonsterCharacter*>();
    for(int i=0;i<monsters.size();i++)
    {
        if(monsters[i]->isAlive())
        {
            monsters[i]->setActive(true);
            connect(monsters[i],&Character::sendPosition,this,&MonsterController::judgeMove);
            connect(monsters[i],&Character::sendRepelPosition,this,&MonsterController::judgeRepel);
            connect(monsters[i],&MonsterCharacter::needMove,this,&MonsterController::moved);
            connect(monsters[i],&MonsterCharacter::needShoot,this,&MonsterController::shooted);
            connect(monsters[i],&Character::dead,this,&MonsterController::dealMonsterDead);
//            moved(monsters[i]);
//            shooted(monsters[i]);
        }
    }

}

void MonsterController::uncontroller(SmallMap *smallMap)
{
    QList<MonsterCharacter*> monsters = smallMap->findChildren<MonsterCharacter*>();
    for(int i=0;i<monsters.size();i++)
    {
        monsters[i]->setActive(false);
        disconnect(monsters[i],0,this,0);
    }
}

void MonsterController::moved(MonsterCharacter *monsterCharacter)
{
    if(!monsterCharacter->isAlive())return;
    QList<Character*> characters = parentWidget()->findChild<Map*>("map")->findChildren<Character*>("",Qt::FindDirectChildrenOnly);
    for(int i =0;i<characters.size();i++)
    {
        QPointF angle;
        if(isSeeCharacter(monsterCharacter,characters[i],angle))
        {
            if(qrand()%100>40)break;
            moveToCharacter(monsterCharacter,angle);
            return;
        }
    }
    if(qrand()%100>40)
    {
        QPointF point =QPointF(0.0,0.0);
        moveToCharacter(monsterCharacter,point);
    }
    else
    {
        int dx = qrand()%200;
        dx-=100;
        int dy = qrand()%200;
        dy-=100;
        QPointF point = QPointF(dx/qSqrt(dx*dx+dy*dy),dy/qSqrt(dx*dx+dy*dy));
        moveToCharacter(monsterCharacter,point);
    }
}

void MonsterController::shooted(MonsterCharacter *monsterCharacter)
{
    if(!monsterCharacter->isAlive())return;
    QList<Character*> characters = parentWidget()->findChild<Map*>("map")->findChildren<Character*>("",Qt::FindDirectChildrenOnly);
    for(int i =0;i<characters.size();i++)
    {
        QPointF angle;
        if(isSeeCharacter(monsterCharacter,characters[i],angle))
        {
            float angle1 = qAtan2(angle.y(),angle.x()) * 180 /M_PI;
            if(qrand()%100>80)break;
            shoot(monsterCharacter,angle1);
            return;
        }
    }
    if(qrand()%100<20)
    {
        shoot(monsterCharacter,monsterCharacter->getWeapon()->angle);
    }
}

void MonsterController::changeWeaponAngle()
{
    QList<MonsterCharacter*> monsters = this->smallMap->findChildren<MonsterCharacter*>();
    for(int i =0;i<monsters.size();i++)
    {
        if(monsters[i]->isActive()&&monsters[i]->isAlive())
        {
           changeWeaponAngle(monsters[i]);
        }
    }
}

void MonsterController::changeWeaponAngle(MonsterCharacter *monsterCharacter)
{
    QList<Character*> characters = parentWidget()->findChild<Map*>("map")->findChildren<Character*>("",Qt::FindDirectChildrenOnly);
    for(int i =0;i<characters.size();i++)
    {
        QPointF angle;
        if(isSeeCharacter(monsterCharacter,characters[i],angle))
        {
            float angle1 = qAtan2(angle.y(),angle.x()) * 180 /M_PI;
            changeWeaponAngle(monsterCharacter,angle1);
            return;
        }
    }
    if(qrand()%100<50)
    {
        float angle1 = qrand()%10;
        angle1-=5;
        changeWeaponAngle(monsterCharacter,monsterCharacter->getWeapon()->angle+angle1);
    }
}

void MonsterController::changeWeaponAngle(MonsterCharacter *monster, float angle)
{
    monster->getWeapon()->setAngle(angle);
}

bool MonsterController::isSeeCharacter(MonsterCharacter *monster, Character *character, QPointF& angle)
{
    QPoint c=QPoint(character->x()%1350,character->y()%1350);
    QPoint d(c.x()-monster->x(),c.y()-monster->y());
    if(distance(QPoint(0,0),d)>600)return false;
    QPoint step(d.x()/20,d.y()/20);
    angle = QPointF(d.x()/qSqrt(d.x()*d.x()+d.y()*d.y()),d.y()/qSqrt(d.x()*d.x()+d.y()*d.y()));
    //    qDebug()<<monster->getPoint()<<c;
    QPoint point = monster->getPoint();
    int i=0;
    while(i++<20)
    {
        point += step;
        if(((SmallMap*)monster->parentWidget())->getState_isMove(point)==Element::_type::BARRIER){
            return false;
        }
    }
    return true;
}

void MonsterController::moveToCharacter(MonsterCharacter *monster, QPointF& angle)
{
    QPointF point = QPointF(angle.x()+0.5,angle.y()+0.5);
    monster->setMoveWay(point);
}

double MonsterController::distance(QPoint x1, QPoint x2)
{
    return qSqrt((x1.x() - x2.x()) * (x1.x() - x2.x()) + (x1.y() - x2.y()) * (x1.y() - x2.y()));
}

void MonsterController::judgeMove(Character *character, QPoint point ,QPoint movePoint)
{
    if(((SmallMap*)character->parentWidget())->getState_isMove(point) != Element::_type::BARRIER){
        character->moveStep(movePoint - QPoint(character->width()/2,character->height()/2+20));
    }
}

void MonsterController::judgeRepel(Character *character, QPoint point)
{
    if(((SmallMap*)character->parentWidget())->getState_isMove(point) != Element::_type::BARRIER){
        character->moveRepel(point - QPoint(character->width()/2,character->height()/2+20));
    }
}



