#ifndef BODYLABEL_H
#define BODYLABEL_H

#include <QLabel>

class BodyLabel : public QLabel
{
    Q_OBJECT
public:
    explicit BodyLabel(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent* e);


signals:

};

#endif // BODYLABEL_H
