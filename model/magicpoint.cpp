#include "magicpoint.h"
#include <QTimer>
#include <QPainter>
#include <QDebug>
#include "tools/myanimation.h"

MagicPoint::MagicPoint(QWidget *parent, QPoint point) : QLabel(parent)
{
    this->value = 4;
    this->imageNumber = 0;
    this->isPicked = false;
    resize(14,14);
    move(point.x()-width()/2,point.y()-height()/2);
    QTimer* timer = new QTimer(this);
    timer->setObjectName("timer");
    connect(timer,&QTimer::timeout,this,[=](){update();});
    timer->start(50);
    show();

    MyAnimation* pPosAnimation1 = new MyAnimation(this, "geometry");
    connect(pPosAnimation1, &QPropertyAnimation::finished, pPosAnimation1, &QPropertyAnimation::deleteLater);
    pPosAnimation1->setDuration(1000);
    int dx,dy;
    dx=qrand()%30;
    dy=qrand()%30;
    dx-=15;
    dy-=15;
    pPosAnimation1->setStartValue(QRect(this->x(), this->y(), this->width(), this->height()));
    pPosAnimation1->setEndValue(QRect(this->x()+dx, this->y()+dy, this->width(), this->height()));
    pPosAnimation1->setEasingCurve(QEasingCurve::OutQuad);
    pPosAnimation1->start();

}

QPoint MagicPoint::getPoint()
{
    return QPoint(x()+width()/2,y()+height()/2);
}

void MagicPoint::setPoint(QPoint point)
{
    setGeometry(point.x()-width()/2,point.y()-height()/2,width(),height());
}

bool MagicPoint::getPicked()
{
    return isPicked;
}

void MagicPoint::setPicked(bool target)
{
    isPicked = target;
}

void MagicPoint::Picked(Character* character)
{
    this->setPicked(true);
    MyAnimation* pPosAnimation1 = new MyAnimation(this, "geometry" ,this);
    pPosAnimation1->setObjectName("pick");
    connect(pPosAnimation1, &QPropertyAnimation::finished,this,[=](){
        disconnect(character,&Character::sendPosition,this,&MagicPoint::trace);
    });
    connect(pPosAnimation1, &QPropertyAnimation::finished, [=](){
        emit pick(character);
    });
    connect(pPosAnimation1, &QPropertyAnimation::finished, pPosAnimation1, &QPropertyAnimation::deleteLater);
    pPosAnimation1->setDuration(2000);
    pPosAnimation1->setStartValue(QRect(this->x(), this->y(), this->width(), this->height()));
    pPosAnimation1->setEndValue(QRect(character->getPoint().x()%1350,character->getPoint().y()%1350,this->width(),this->height()));
    pPosAnimation1->setEasingCurve(QEasingCurve::InQuad);
    pPosAnimation1->start();
    connect(character,&Character::sendPosition,this,&MagicPoint::trace);
}

void MagicPoint::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    /* 碟机转动 */
    if(++imageNumber == 5)
    {
        imageNumber = 1;
    }
    /* 画图操作 */
    painter.drawPixmap(rect(),QPixmap(QString("resource\\image\\things\\magic%1.png").arg(imageNumber)));
    painter.end();

}

void MagicPoint::stopGame()
{
    QTimer* timer = findChild<QTimer*>("timer");
    timer->stop();
}

void MagicPoint::continueGame()
{
    QTimer* timer = findChild<QTimer*>("timer");
    timer->start();
}

void MagicPoint::trace(Character *character)
{
    MyAnimation* pPosAnimation1 = findChild<MyAnimation*>("pick");
    pPosAnimation1->setEndValue(QRect(character->getPoint().x()%1350,character->getPoint().y()%1350,this->width(),this->height()));
}
