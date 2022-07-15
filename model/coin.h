#ifndef COIN_H
#define COIN_H

#include <QWidget>
#include <QLabel>

#include "model/character.h"

class Coin : public QLabel
{
    Q_OBJECT
public:
    explicit Coin(QWidget *parent,QPoint point);

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

#endif // COIN_H
