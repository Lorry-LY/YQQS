#include "weapon.h"
#include <QMetaEnum>
#include <QDebug>
#include <QPainter>
#include <QtMath>

Weapon::Weapon(QWidget *parent) : QLabel(parent)
{

}

Weapon::Weapon(QWidget *parent,QString name, int ATK, int MP, float speed, float crit, int type,QString weaponImage, QString bulletImage,float bulletSpeed)
{
    setAttribute(Qt::WA_TranslucentBackground, false);
    this->angle = 0;
    this->setParent(parent);
    this->name = name;
    this->ATK = ATK;
    this->spell = MP;
    this->speed = speed;
    this->enable = true;
    this->crit = crit;
    this->type = Weapon::_type(type);
    //    qDebug()<<this->type;
    //    this->type = (Weapon::type)metaEnum.valueToKey(1);
    setPixmap(QPixmap(weaponImage));
    this->bulletImage = bulletImage;
    this->bulletSpeed = bulletSpeed;
    //    this->setScaledContents(true);
    this->resize(70,70);
    setScaledContents(true);

}

QPoint Weapon::getPoint()
{
    return QPoint(x()+width()/2,y()+height()/2);
}

void Weapon::setPoint(QPoint point)
{
    setGeometry(point.x()-width()/2,point.y()-height()/2,width(),height());
}

float Weapon::getAngle()
{
    return angle;
}

void Weapon::setAngle(float angle)
{
    this->angle = angle;
}

void Weapon::setAngle()
{
    QPoint point = mapToGlobal(QPoint());
    point = cursor().pos() - point;
    this->angle = qAtan2(point.y(),point.x()) * 180 / M_PI;
    update();
}

unsigned Weapon::getSpell()
{
    return spell;
}

void Weapon::setEnable(bool enable)
{
    this->enable = enable;
}

bool Weapon::getEnable()
{
    return this->enable;
}

void Weapon::paintEvent(QPaintEvent *e)
{
    QImage image(pixmap()->toImage());
    QImage mirroredImage = image.mirrored(false, true);
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::NoBrush);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    QPixmap pixmap;
    if(cos(angle * M_PI / 180)<0)
    {
        pixmap = QPixmap::fromImage(mirroredImage);
    }
    else
    {
        pixmap = QPixmap::fromImage(image);
    }
    painter.translate(this->width() / 2, this->height() / 2);          //设置旋转中心
    painter.rotate(angle);          //旋转
    painter.translate(-(this->width() / 2), -(this->height() / 2));        //将原点复位
    painter.drawPixmap(this->rect(), pixmap);
    painter.end();
//    setPixmap(*pixmap());
//    QWidget::paintEvent(e);
}
