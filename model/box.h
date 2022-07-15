#ifndef BOX_H
#define BOX_H

#include <QWidget>
#include <QLabel>

class Box : public QLabel
{
    Q_OBJECT
public:
    explicit Box(QWidget *parent = nullptr);
    Box(QWidget *parent, int pay, int HP, int MP);
    QPoint getPoint();
    void setPoint(QPoint point);

public:
    int pay;        //花费
    int HP;         //生命值加成
    int MP;         //能量加成

};

#endif // BOX_H
