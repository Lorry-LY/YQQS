#ifndef SETTINGWIDGET_H
#define SETTINGWIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTextEdit>
#include <QMouseEvent>

namespace Ui {
class SettingWidget;
}




class SettingWidget :public QWidget
{
    Q_OBJECT

public:
    explicit SettingWidget(QWidget *parent = nullptr);
    ~SettingWidget();

    void init();
signals:
    void changeBGMvolume(int volume);
    void changeSoundVolume(int volume);
    void finished();
private slots:
    void on_Save_clicked();
    void on_Back_clicked();

public:
    QMap<QString,QString> keys;

private:
    Ui::SettingWidget *ui;
    QSqlDatabase db;

};

#endif // SETTINGWIDGET_H
