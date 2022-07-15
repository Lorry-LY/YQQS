#include "character.h"
#include <QHBoxLayout>
#include <QSlider>
#include <QStackedLayout>
#include <QProgressBar>
#include <QTimeLine>
#include <QPainter>
#include <QtMath>
#include <QTimer>
#include <QDebug>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

Character::Character(QWidget *parent) : QWidget(parent)
{

}

Character::Character(QWidget *parent,QString name, int maxHP, int maxMP, int maxShield, QString bodyURL,  Weapon* weapon, float speed): QWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, false);
    bodyImage.setParent(this);
    this->alive = true;
    this->gameStop = false;
    this->name = name;
    this->MaxHP = maxHP;
    this->MaxMP = maxMP;
    this->MaxShield = maxShield;
    this->speed = speed;
    this->skillEnable = true;
    this->isSkillTime = false;
    QHBoxLayout* layout  = new QHBoxLayout(&headWidget);
    QProgressBar* slider = new QProgressBar(&headWidget);
    slider->setObjectName("slider");
    slider->setMaximum(maxHP);
    slider->setMinimum(0);
    slider->setValue(maxHP);
    slider->setStyleSheet("QProgressBar{border:2px solid rgb(100,100,100);"
                          "background: rgb(100,100,100);"
                          "color:rgb(255,0,0);}"
                          );
    slider->setTextVisible(false);
    slider->setEnabled(false);
    //    layout->setMargin(0);
    layout->addWidget(slider);
    headWidget.setLayout(layout);
    this->bodyURL = bodyURL;
    this->weapon = weapon;
    this->speed = speed;
    HP = maxHP;
    MP = maxMP;
    shield = maxShield;
    coin = 0;
    state = _state::STOP;
    resize(100,120);
    for (int i=0;i<4;i++) moveWay[i]=0;
    headWidget.setParent(this);
    headWidget.setGeometry(0,0,100,30);
    bodyImage.setPixmap(QPixmap(bodyURL+"\\stop1.png"));
    bodyImage.setParent(this);
    bodyImage.setGeometry(15,30,70,70);
    bodyImage.setScaledContents(true);
    if(weapon != nullptr){
        weapon->setParent(this);
        weapon->setGeometry(15,50,weapon->width(),weapon->height());
    }
    QTimer* updateBody = new QTimer(this);
    updateBody->setObjectName("updateBody");
    updateBody->setInterval(500);
    connect(updateBody,&QTimer::timeout,this,[=](){
        if(!isSkillTime){
            if(state == Character::MOVE){
                if(*bodyImage.pixmap()!=QPixmap(bodyURL+"\\stop1.png")){
                    bodyImage.setPixmap(QPixmap(bodyURL+"\\stop1.png"));
                }else{
                    bodyImage.setPixmap(QPixmap(bodyURL+"\\run.png"));
                }
            }else{
                if(*bodyImage.pixmap()!=QPixmap(bodyURL+"\\stop1.png")){
                    bodyImage.setPixmap(QPixmap(bodyURL+"\\stop1.png"));
                }else{
                    bodyImage.setPixmap(QPixmap(bodyURL+"\\stop2.png"));
                }
            }
        }else{
            if(*bodyImage.pixmap()!=QPixmap(bodyURL+"\\skill1.png")){
                bodyImage.setPixmap(QPixmap(bodyURL+"\\skill1.png"));
            }else{
                bodyImage.setPixmap(QPixmap(bodyURL+"\\skill2.png"));
            }
        }
    });
    updateBody->start();

    QTimeLine* timeLine = new QTimeLine(40000, this);
    timeLine->setFrameRange(0, 9000);
    timeLine->setObjectName("timeLine");
    timeLine->setLoopCount(0);
    timeLine->setCurveShape(QTimeLine::LinearCurve);
    timeLine->setUpdateInterval(100 / this->speed);
    connect(timeLine, &QTimeLine::frameChanged, this, [=](int frame) {
        if(!this->gameStop)
        {
            QPoint point(x()+width()/2 + 2.0 * (moveWay[2]+moveWay[3]),
                    y()+height()/2+20 + 2.0 * (moveWay[0]+moveWay[1]));
            QPoint movePoint = point + QPoint(25.0 * (moveWay[2]+moveWay[3]),25.0 * (moveWay[0]+moveWay[1]));
            if(point != getPoint()){
                state = Character::MOVE;
            }else{
                state = Character::STOP;
            }
            if(movePoint!=QPoint(x()+width()/2,y()+height()/2+20)){
                //                qDebug()<<frame<<point<<movePoint<<timeLine->updateInterval();
            }
            emit sendPosition(this,movePoint,point);
        }
    });
    timeLine->start();

    this->recoverTime = 0;
    QTimer* recoverTimer = new QTimer(this);
    recoverTimer->setObjectName("recoverTimer");
    connect(recoverTimer,&QTimer::timeout,this,[=](){
        if(recoverTime != 0 )recoverTime--;
        else{
            changeShield(1);
        }
    });
    recoverTimer->start(1000);

}

QPoint Character::getPoint()
{
    return QPoint(x()+width()/2,y()+height()/2+20);
}

void Character::setPoint(QPoint point)
{
    setGeometry(point.x(),point.y(),width(),height());
}

Weapon *Character::getWeapon()
{
    return weapon;
}

void Character::setWeapon(Weapon *weapon)
{
    this->weapon = weapon;
}

unsigned Character::getHP()
{
    return HP;
}

void Character::setHP(int HP)
{
    this->HP = HP;
}

void Character::changeHP(int HP)
{
    this->HP = this->HP + HP > this->MaxHP ? this->MaxHP : this->HP + HP;
    emit changedHP(this->HP,this->MaxHP);
    QProgressBar* slider = headWidget.findChild<QProgressBar*>("slider");
    slider->setValue(this->HP);
}

unsigned Character::getMP()
{
    return MP;
}

void Character::setMP(int MP)
{
    this->MP = MP;
}

void Character::changeMP(int MP)
{
    this->MP = this->MP + MP > this->MaxMP ? this->MaxMP : this->MP + MP;
    emit changedMP(this->MP,this->MaxMP);
}

unsigned Character::getShield()
{
    return shield;
}

void Character::setShield(int Shield)
{
    this->shield = Shield;
}

void Character::changeShield(int Shield)
{
    this->shield = this->shield + Shield > this->MaxShield ? this->MaxShield : this->shield + Shield;
    emit changedShied(this->shield,this->MaxShield);
}

unsigned Character::getCoin()
{
    return this->coin;
}

void Character::setCoin(int coin)
{
    this->coin = coin;
}

void Character::changeCoin(int coin)
{
    this->coin += coin;
    emit changedCoin(this->coin);
}

void Character::hitted(int ATK, float repelAngle)
{
    if(ATK >= HP + shield)
    {
        changeHP(-HP);
        changeShield(-shield);
        emit dead(this);
        death();
        return;
    }
    if(ATK <= shield)
    {
        changeShield(-ATK);
        recoverTime = 4;
    }
    else if(shield<ATK&&ATK<HP + shield)
    {
        recoverTime = 4;
        changeHP(- ATK + shield);
        changeShield(-shield);
    }

    QPoint point(x()+width()/2,y()+height()/2+20);
    QPoint movePoint = point + QPoint(20.0 * cos(repelAngle * M_PI / 180), 20.0 * sin(repelAngle * M_PI / 180));
    emit sendRepelPosition(this,movePoint);

}

void Character::changeMoveWay(int way, int value)
{
    if(isAlive())
    {
        this->moveWay[way] = value;
    }
}

void Character::moveStep(QPoint point)
{
    this->move(point);
}

void Character::moveRepel(QPoint point)
{
    QPropertyAnimation* pPosAnimation1 = new QPropertyAnimation(this, "geometry" ,this);
    pPosAnimation1->setObjectName("pPosAnimation1");
    connect(pPosAnimation1, &QPropertyAnimation::finished, pPosAnimation1, &QPropertyAnimation::deleteLater);
    pPosAnimation1->setDuration(200);
    pPosAnimation1->setStartValue(QRect(this->x(), this->y(), this->width(), this->height()));
    pPosAnimation1->setEndValue(QRect(point.x(),point.y(),width(),height()));
    pPosAnimation1->setEasingCurve(QEasingCurve::Linear);
    pPosAnimation1->start();
}

void Character::setAlive(bool alive)
{
    this->alive = alive;
}

bool Character::isAlive()
{
    return this->alive;
}

void Character::stopGame()
{
    this->gameStop = true;
    QTimer* timer1= findChild<QTimer*>("updateBody");
    QTimer* timer2= findChild<QTimer*>("recoverTimer");
    if(timer1){
        timer1->stop();
    }
    if(timer2){
        timer2->stop();
    }
    QPropertyAnimation* pPosAnimation1= findChild<QPropertyAnimation*>("pPosAnimation1");
    if(pPosAnimation1){
        pPosAnimation1->pause();
    }
}

void Character::continueGame()
{
    this->gameStop = false;
    QTimer* timer1= findChild<QTimer*>("updateBody");
    QTimer* timer2= findChild<QTimer*>("recoverTimer");
    if(timer1){
        timer1->start();
    }
    if(timer2){
        timer2->start();
    }
    QPropertyAnimation* pPosAnimation1= findChild<QPropertyAnimation*>("pPosAnimation1");
    if(pPosAnimation1){
        pPosAnimation1->resume();
    }
}

void Character::death()
{
    setAlive(false);
    QTimer* timer1= findChild<QTimer*>("updateBody");
    QTimer* timer2= findChild<QTimer*>("recoverTimer");
    for(int i=0;i<4;i++)moveWay[i]=0;
    if(timer1){
        delete timer1;
    }
    if(timer2){
        delete timer2;
    }
    bodyImage.setPixmap(bodyURL+"\\dead.png");
    headWidget.hide();
    weapon->hide();

    QSequentialAnimationGroup *pPosGroup = new QSequentialAnimationGroup(this);
    connect(pPosGroup,&QSequentialAnimationGroup::finished,pPosGroup,&QSequentialAnimationGroup::deleteLater);
    pPosGroup->addPause(0);
    QPropertyAnimation *pPosAnimation1 = new QPropertyAnimation(this, "pos" ,pPosGroup);
    pPosAnimation1->setDuration(800);
    pPosAnimation1->setStartValue(pos());
    pPosAnimation1->setEndValue(pos()+QPoint(0,-20));
    pPosAnimation1->setEasingCurve(QEasingCurve::OutQuad);
    QPropertyAnimation *pPosAnimation2 = new QPropertyAnimation(this, "pos" ,pPosGroup);
    pPosAnimation2->setDuration(800);
    pPosAnimation2->setStartValue(pos()+QPoint(0,-20));
    pPosAnimation2->setEndValue(pos());
    pPosAnimation2->setEasingCurve(QEasingCurve::InQuad);
    pPosGroup->addAnimation(pPosAnimation1);
    pPosGroup->addAnimation(pPosAnimation2);
    pPosGroup->start();
}



