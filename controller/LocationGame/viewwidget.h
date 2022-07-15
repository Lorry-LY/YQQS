#ifndef VIEWWIDGET_H
#define VIEWWIDGET_H

#include <QWidget>
#include <model/map.h>

class ViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ViewWidget(QWidget *parent = nullptr);

public:
    void updateMap();
    void moveView(QPoint center);

signals:

public:
    Map* map;
};

#endif // VIEWWIDGET_H
