#ifndef STOPWIDGET_H
#define STOPWIDGET_H

#include <QWidget>

namespace Ui {
class StopWidget;
}

class StopWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StopWidget(QWidget *parent = nullptr);
    ~StopWidget();

signals:
    void BackToHall();
    void Setting();
    void Continue();


private slots:
    void on_PushButton_Continue_clicked();

    void on_PushButton_BackToHall_clicked();

    void on_PushButton_Setting_clicked();

private:
    Ui::StopWidget *ui;
};

#endif // STOPWIDGET_H
