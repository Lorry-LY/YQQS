#ifndef MESSAGE_H
#define MESSAGE_H

#include <QLabel>

class Message : public QLabel
{
    Q_OBJECT
public:
    explicit Message(QWidget *parent,QPoint point,QString text,QColor color);

    void disappear();

signals:

};

#endif // MESSAGE_H
