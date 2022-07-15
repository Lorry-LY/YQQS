#ifndef INPUTTING_H
#define INPUTTING_H

#include <QObject>
#include <QKeyEvent>

class Inputting : public QObject
{
    Q_OBJECT
public:
    explicit Inputting(QObject *parent = nullptr);

    void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);


signals:
    void move(char way);
    void shoot();

};

#endif // INPUTTING_H
