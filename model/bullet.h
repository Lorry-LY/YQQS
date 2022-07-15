#ifndef BULLET_H
#define BULLET_H

#include <QWidget>
#include <QLabel>

class Bullet : public QLabel
{
    Q_OBJECT
public:
    explicit Bullet(QWidget *parent = nullptr);
    Bullet(QWidget *parent,unsigned ATK,float shootAngle,float speed,QString URL,QPoint point,int offset);

    QPoint getPoint();
    void setPoint(QPoint point);

    void moveStart();

    void hitBarrier();
    void hitMonster();

    void stopGame();
    void continueGame();

signals:
    void sendPosition(Bullet* bullet);

protected:
    void paintEvent(QPaintEvent *e);

public:
    unsigned ATK;       //伤害量
    float shootAngle;   //射击角
    float speed;        //速度
    QPoint point;
};

#endif // BULLET_H
