#ifndef STARTWIDGET_H
#define STARTWIDGET_H

#include <QWidget>


namespace Ui {
class StartWidget;
}

class StartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StartWidget(QWidget *parent = nullptr);
    ~StartWidget();

signals:
    void setting();
    void location();
    void online();

private slots:
    void on_setting_clicked();
    void on_Location_clicked();
    void on_Online_clicked();

private:
    Ui::StartWidget *ui;

};

#endif // STARTWIDGET_H
