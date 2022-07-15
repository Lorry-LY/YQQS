#include "charactercontroller.h"
#include "model/map.h"
#include <QDebug>
#include <QGraphicsView>
#include <QtMath>
#include <QTimer>

CharacterController::CharacterController(QWidget* parent) : QWidget(parent)
{
     media = this->parentWidget()->parentWidget()->parentWidget()->findChild<MediaController*>("media");

     timer1=new QTimer(this);
     timer2=new QTimer(this);

     connect(timer1,SIGNAL(timeout()),this,SLOT(run()));
     connect(timer2,SIGNAL(timeout()),this,SLOT(wait()));

}

void CharacterController::createCharacter(Character* character)
{
	characters.push_back(character);
	myCharacter = character;
}

Character* CharacterController::getMyCharacter()
{
	return myCharacter;
}

void CharacterController::move(Character* c, char moveWay)
{
	switch (moveWay) {
	case 'W':
        c->changeMoveWay(0,-1);
		break;
	case 'S':
        c->changeMoveWay(1,1);
		break;
	case 'A':
        c->changeMoveWay(2,-1);
		break;
	case 'D':
        c->changeMoveWay(3,1);
		break;
	}
    emit changeView(c->getPoint());
}

void CharacterController::stop(Character *character, char moveWay)
{
    switch (moveWay) {
    case 'W':
        character->changeMoveWay(0,0);
        break;
    case 'S':
        character->changeMoveWay(1,0);
        break;
    case 'A':
        character->changeMoveWay(2,0);
        break;
    case 'D':
        character->changeMoveWay(3,0);
        break;
    }
    emit changeView(character->getPoint());
}

void CharacterController::shoot(Character* character)
{
    if (!character->isAlive())return;
	if (character->getWeapon()->getSpell() > character->getMP())return;
	if (character->getWeapon()->getEnable() == false)return;
	character->getWeapon()->setEnable(false);
	QTimer::singleShot(character->getWeapon()->speed, this, [=]() {character->getWeapon()->setEnable(true); });
    character->changeMP(-character->getWeapon()->getSpell());
	int ATK = qrand() % 100 > character->getWeapon()->crit * 100 ? character->getWeapon()->ATK * 2 : character->getWeapon()->ATK;
	float angle = character->getWeapon()->angle;
	float speed = character->getWeapon()->bulletSpeed;
	Bullet* bullet = new Bullet((QWidget*)parentWidget()->findChild<Map*>("map"), ATK, angle, speed, character->getWeapon()->bulletImage, character->getPoint(), character->getWeapon()->width() / 2);
    QMediaPlayer *shot = new QMediaPlayer(this);
    shot->setMedia(media->findChild<QMediaPlayer*>("枪声")->media());
    shot->setVolume(media->findChild<QMediaPlayer*>("枪声")->volume());
    shot->play();
    connect(shot,&QMediaPlayer::stateChanged,shot,&QMediaPlayer::deleteLater);
    emit addBullet(character,bullet);
}

void CharacterController::changeWeaponAngle(Character* character)
{
    if (character->getWeapon()->type != Weapon::GUN)return;
    character->getWeapon()->setAngle();

}

void CharacterController::changeWeaponAngle(Character* character, float angle)
{
    if (character->getWeapon()->type != Weapon::GUN)return;
    character->getWeapon()->setAngle();

}

void CharacterController::changeWeapon(Character *character)
{
    QList<Weapon*> weapons = character->parentWidget()->findChildren<Weapon*>();
    while (!weapons.isEmpty())
    {
        Weapon* weapon = weapons.front();
        weapons.pop_front();
        if(distance(weapon->getPoint(),character->getPoint())<30)
        {
            changeWeapon(character,weapon);
            weapons.clear();
            break;
        }
    }
}

void CharacterController::changeWeapon(Character *character, Weapon* weapon)
{
    QPoint point(weapon->getPoint());
    Weapon* old = character->getWeapon();
    old->setParent(character->parentWidget());
    old->setPoint(point);
    old->show();
    character->setWeapon(weapon);
    weapon->setParent(character);
    weapon->setGeometry(15,50,weapon->width(),weapon->height());
    weapon->show();
    emit changeWeapon(old);
}

void CharacterController::buyBox(Character *character)
{
    QList<Box*> boxs = character->parentWidget()->findChildren<Box*>();
    while (!boxs.isEmpty())
    {
        Box* box = boxs.front();
        boxs.pop_front();
        QPoint point(character->getPoint().x()%1350,character->getPoint().y()%1350);
        if(distance(box->getPoint(),point)<60)
        {
            if(character->getCoin()>=box->pay)
            {
                QMediaPlayer *shot = new QMediaPlayer(this);
                shot->setMedia(media->findChild<QMediaPlayer*>("购买")->media());
                shot->setVolume(media->findChild<QMediaPlayer*>("购买")->volume());
                shot->play();
                connect(shot,&QMediaPlayer::stateChanged,shot,&QMediaPlayer::deleteLater);
                buyBox(character,box);
                boxs.clear();
                break;
            }
        }
    }
}

void CharacterController::buyBox(Character *character, Box* box)
{
    character->changeHP(box->HP);
    character->changeMP(box->MP);
    character->changeCoin(-box->pay);
    delete box;
}

void CharacterController::victory(Character *character)
{
    QLabel* item = character->parentWidget()->findChild<QLabel*>("传送门");
    QPoint point1(item->pos().x()+item->width()/2,item->pos().y()+item->height()/2);
    QPoint point(character->getPoint().x()%1350,character->getPoint().y()%1350);
    if(distance(point1,point)<300){
        emit Victory();
    }
}

void CharacterController::releaseSkill(Character *character)
{
    if (character->skillEnable){
        character->skillEnable=false;
        character->isSkillTime = true;
        if (character->name=="法师"){
            character->getWeapon()->spell=0;
        }
        else{
            character->getWeapon()->ATK+=2;
        }
        timer1->start(1000);
    }
}

void CharacterController::run()
{
    if (skillTime!=0){
        skillTime--;
    }
    else {
        if (this->getMyCharacter()->name=="法师"){
            this->getMyCharacter()->getWeapon()->spell=1;
        }
        else{
            this->getMyCharacter()->getWeapon()->ATK-=2;
        }
        getMyCharacter()->isSkillTime = false;
        skillTime=5;
        timer1->stop();
        timer2->start(2000);
    }
}

void CharacterController::wait()
{
    this->getMyCharacter()->skillEnable=true;
    timer2->stop();
}

void CharacterController::dealChangeWeaponAngle(float angle)
{
    myCharacter->weapon->setAngle(angle);
}

void CharacterController::dealCharacterDead(Character *character)
{
    QTimer::singleShot(3000,this,[=](){
        emit GameOver();
    });
}

void CharacterController::judgeMove(Character *character, QPoint point ,QPoint movePoint)
{
//    qDebug()<<this->parentWidget()->findChild<Map*>("map");
//    qDebug() << point;
    if(this->parentWidget()->findChild<Map*>("map")->getState_isMove(point) != Element::_type::BARRIER){
        character->moveStep(movePoint - QPoint(character->width()/2,character->height()/2+20));
    }
}

void CharacterController::judgeRepel(Character *character, QPoint point)
{
    if(this->parentWidget()->findChild<Map*>("map")->getState_isMove(point) != Element::_type::BARRIER){
        character->moveRepel(point - QPoint(character->width()/2,character->height()/2+20));
    }
}

void CharacterController::stopGame()
{
    timer1->stop();
    timer2->stop();
}

void CharacterController::continueGame()
{
    timer1->start();
    timer2->start();
}

double CharacterController::distance(QPoint x1, QPoint x2)
{
    return qSqrt((x1.x() - x2.x()) * (x1.x() - x2.x()) + (x1.y() - x2.y()) * (x1.y() - x2.y()));
}



