#ifndef ELEMENT_H
#define ELEMENT_H

#include <QWidget>
#include <QLabel>

class Element : public QLabel
{
    Q_OBJECT
public:
    explicit Element(QWidget *parent = nullptr);

    void changeToBarrier();
    void changeToFlat();
    void closeDoor();
    void openDoor();

    QPoint getPoint();

    void init(int typeID);

    void update(int type);

    void setState(int state);
    void setType(int state);

    int getState_isMove();
    int getState_isShoot();

signals:

public:
    enum _type{
        NONE = 0,
        FLAT = 1,
        BARRIER = 2,
        DOOR = 3
    }type;

    enum _state{
        OPEN = 1,
        CLOSE = 2
    }state;

    Q_ENUM(_type)
    Q_ENUM(_state)

    QPoint point;


};

#endif // ELEMENT_H
