#include "monstercharacter.h"
#include <QDebug>
#include <QTimer>

MonsterCharacter::MonsterCharacter(QWidget* parent)
{
    this->tagActive=true;
    this->setParent(parent);
}

MonsterCharacter::MonsterCharacter(QWidget* parent, QString name, int maxHP, int maxMP, int maxShield, QString bodyURL, Weapon* weapon, float speed)
    : Character(parent,name,maxHP,maxMP,maxShield,bodyURL,weapon,speed)
{
    this->tagActive=true;
    this->coin = 0;
}

MonsterCharacter::MonsterCharacter(QWidget* parent, QString name, int maxHP, int maxMP, int maxShield, int coin, QString bodyURL, Weapon* weapon, float speed)
    : Character(parent, name, maxHP, maxMP, maxShield, bodyURL, weapon, speed)
{
    this->tagActive = false;
    this->coin = coin;
    this->headWidget.hide();

    QTimer* timer1 = new QTimer(this);
    timer1->setObjectName("timer1");
    connect(timer1,&QTimer::timeout,this,[=](){
        emit needMove(this);
    });
    int d1 = qrand()%1000;
    d1+=1500;
    timer1->start(d1);

    QTimer* timer2 = new QTimer(this);
    timer2->setObjectName("timer2");
    connect(timer2,&QTimer::timeout,this,[=](){
        emit needShoot(this);
    });
    int d2 = qrand()%1000;
    d2+=2000;
    timer2->start(d2);

}





bool MonsterCharacter::isActive()
{
    return tagActive;
}

void MonsterCharacter::setActive(bool active)
{
    this->tagActive = active;
}

void MonsterCharacter::setMoveWay(QPointF point)
{
    this->moveWay[2]=0;
    this->moveWay[3]=point.x();
    this->moveWay[0]=0;
    this->moveWay[1]=point.y();
}

void MonsterCharacter::stopGame()
{
    Character::stopGame();
    QTimer* timer1= findChild<QTimer*>("timer1");
    QTimer* timer2= findChild<QTimer*>("timer2");
    if(timer1){
        timer1->stop();
    }
    if(timer2){
        timer2->stop();
    }

}

void MonsterCharacter::continueGame()
{
    Character::continueGame();
    QTimer* timer1= findChild<QTimer*>("timer1");
    QTimer* timer2= findChild<QTimer*>("timer2");
    if(timer1){
        timer1->start();
    }
    if(timer2){
        timer2->start();
    }
}
