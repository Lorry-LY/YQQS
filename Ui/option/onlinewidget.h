#ifndef ONLINEWIDGET_H
#define ONLINEWIDGET_H

#include <QWidget>

namespace Ui {
class OnlineWidget;
}

class OnlineWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OnlineWidget(QWidget *parent = nullptr);
    ~OnlineWidget();

private:
    Ui::OnlineWidget *ui;
};

#endif // ONLINEWIDGET_H
