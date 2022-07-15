#include "element.h"
#include <QDebug>

Element::Element(QWidget *parent): QLabel(parent){
    setFixedSize(30, 30);
    setContentsMargins(0,0,0,0);
    setAttribute(Qt::WA_TranslucentBackground, false);
    //setCursor(QCursor(Qt::UpArrowCursor));
    setScaledContents(true);
}

void Element::changeToBarrier()
{
    if(type == Element::_type::FLAT)
    {
        type = Element::_type::BARRIER;
    }
}

void Element::changeToFlat()
{
    if(type == Element::_type::BARRIER){
        type = Element::_type::FLAT;
    }
}

void Element::closeDoor()
{
    if(type == Element::_type::DOOR && state == Element::_state::OPEN){
        state = Element::_state::CLOSE;
        setPixmap(QPixmap("qrc:/resource/image/map/door_close.png"));
    }
}

void Element::openDoor()
{
    if(type == Element::_type::DOOR && state == Element::_state::CLOSE){
        state = Element::_state::OPEN;
        setPixmap(QPixmap("qrc:/resource/image/map/door_close.png"));
    }
}

QPoint Element::getPoint()
{
    return QPoint(geometry().x()+width()/2,geometry().y()+height()/2);
}

void Element::init(int _typeID)
{
//    qDebug()<<"init"<<_typeID;
    this->type = Element::_type(_typeID);
}

void Element::update(int type)
{
//    qDebug()<<"update"<<type;
    state = Element::_state::OPEN;
    this->type = Element::_type(type);
    switch (this->type) {
    case Element::_type::NONE:
        setPixmap(QPixmap(""));
        break;
    case Element::_type::DOOR:{
        setPixmap(QPixmap("resource/image/map/door_open.png"));
        break;
    }
    case Element::_type::FLAT:{
        int number = qrand()%3+1;
        setPixmap(QPixmap(QString("resource/image/map/flat%1.png").arg(number)));
        break;}
    case Element::_type::BARRIER:
        setPixmap(QPixmap("resource/image/map/wall.png"));
        break;
    }
}

void Element::setState(int state)
{
    this->state = Element::_state(state);
    if(this->state == Element::_state::OPEN)
    {
        setPixmap(QPixmap("resource/image/map/door_open.png"));
    }
    else
    {
        setPixmap(QPixmap("resource/image/map/door_close.png"));
    }
}

void Element::setType(int state)
{
    this->type = Element::_type(state);
}

int Element::getState_isMove()
{
//    qDebug()<<this->type;
    //qDebug()<<"element" << this->geometry();
    if(this->type == Element::_type::DOOR)
    {
        if(this->state == Element::_state::CLOSE){
            return Element::_type::BARRIER;
        }else{
            return Element::_type::FLAT;
        }
    }
    if(this->type == Element::_type::NONE)return Element::_type::BARRIER;
    if(this->type == Element::_type::BARRIER)return Element::_type::BARRIER;
    return Element::_type::FLAT;
}

int Element::getState_isShoot()
{
    if(this->type == Element::_type::DOOR)return Element::_type::BARRIER;
    return this->type;
}
