#include "message.h"
#include <QPropertyAnimation>
#include <QDebug>

Message::Message(QWidget *parent, QPoint point, QString text, QColor color) : QLabel(parent)
{
    resize(50,30);
    move(point-QPoint(width()/2,height()/2));
    setText(text);
    //设置字号
    QFont font("Microsoft YaHei", 16, 50);
    setFont(font);
    //设置颜色
    QPalette pa;
    pa.setColor(QPalette::WindowText,color);
    setPalette(pa);

    show();

}

void Message::disappear()
{
    QPropertyAnimation* pPosAnimation1 = new QPropertyAnimation(this, "geometry", this);
    connect(pPosAnimation1, &QPropertyAnimation::finished, this, &Message::deleteLater);
    pPosAnimation1->setDuration(1500);
    pPosAnimation1->setStartValue(QRect(this->x(), this->y(), this->width(), this->height()));
    pPosAnimation1->setEndValue(QRect(this->x(),this->y()-30,width(),height()));
    pPosAnimation1->setEasingCurve(QEasingCurve::OutQuad);
    pPosAnimation1->start();
}
