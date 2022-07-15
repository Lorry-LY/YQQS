#include "inputting.h"

Inputting::Inputting(QObject *parent) : QObject(parent)
{

}

void Inputting::keyPressEvent(QKeyEvent* e)
{
    switch (e->key()) {
    case Qt::Key_W:
        emit move('W');
        break;
    case Qt::Key_S:
        emit move('S');
        break;
    case Qt::Key_A:
        emit move('A');
        break;
    case Qt::Key_D:
        emit move('D');
        break;
    }
}

void Inputting::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        emit shoot();
        //bullets->updateBullets();
    }
}
