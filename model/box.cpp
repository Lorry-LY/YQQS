#include "box.h"

Box::Box(QWidget *parent) : QLabel(parent)
{

}

Box::Box(QWidget *parent, int pay, int HP, int MP) : QLabel(parent)
{
    this->pay = pay;
    this->HP = HP;
    this->MP = MP;
}

QPoint Box::getPoint()
{
    return QPoint(x()+width()/2,y()+height()/2);
}

void Box::setPoint(QPoint point)
{
    setGeometry(point.x()-width()/2,point.y()-height()/2,width(),height());
}
