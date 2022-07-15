#ifndef ENDWIDGET_H
#define ENDWIDGET_H

#include <QWidget>

namespace Ui {
class EndWidget;
}

class EndWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EndWidget(QWidget *parent = nullptr);
    ~EndWidget();
    void init(int time,QPixmap pixmap,int GoldNum,int Level,QString Result);


signals:
    void BackToHall();

private slots:
    void on_PushButton_BackToHall_clicked();

private:
    Ui::EndWidget *ui;
};

#endif // ENDWIDGET_H
