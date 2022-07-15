#include "viewwidget.h"

ViewWidget::ViewWidget(QWidget *parent) : QWidget(parent)
{
    Map* m = new Map(this);
    m->setObjectName("map");
    setWindowState(Qt::WindowFullScreen);
    m->setWindowState(Qt::WindowFullScreen);

}

void ViewWidget::updateMap()
{

}

void ViewWidget::moveView(QPoint center)
{
    this->setGeometry(-center.x()-width()/2,-center.y()-height()/2,width(),height());
}
