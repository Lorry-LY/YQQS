#ifndef MAGICPOINT_H
#define MAGICPOINT_H

#include <QWidget>
#include <QLabel>

#include "model/character.h"

class MagicPoint : public QLabel
{
    Q_OBJECT
public:
    explicit MagicPoint(QWidget *parent,QPoint point);

    QPoint getPoint();
    void setPoint(QPoint point);

    bool getPicked();
    void setPicked(bool target);

    void Picked(Character* character);

    void stopGame();
    void continueGame();

signals:
    void pick(Character* character);

protected:
    void paintEvent(QPaintEvent *);

    void trace(Character* character);

public:
    int value;

private:
    int imageNumber;
    bool isPicked;
};

#endif // MAGICPOINT_H
