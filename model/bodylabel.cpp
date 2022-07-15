#include "bodylabel.h"
#include <QPainter>
#include <QtMath>
#include "model/character.h"

BodyLabel::BodyLabel(QWidget *parent) : QLabel(parent)
{

}

void BodyLabel::paintEvent(QPaintEvent* e)
{
    QImage image(pixmap()->toImage());
    QImage mirroredImage = image.mirrored(true, false);
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::NoBrush);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    if(((Character*)parentWidget())->getWeapon())
    {
        if(cos(((Character*)parentWidget())->getWeapon()->angle * M_PI / 180)<0)
        {
            painter.drawPixmap(this->rect(), QPixmap::fromImage(mirroredImage));
        }
        else
        {
            painter.drawPixmap(this->rect(), QPixmap::fromImage(image));
        }
    }
    painter.end();
}
