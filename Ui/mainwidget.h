#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "option/startwidget.h"
#include "option/settingwidget.h"
#include "option/onlinewidget.h"
#include "option/locationwidget.h"
#include "controller/mediaController/mediacontroller.h"
#include <QSqlDatabase>
#include <QSqlQuery>


namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();


private:
    void dealBack();
    void dealSetting();
    void dealLocation();
    void dealOnline();
    void dealChangeBGMvolume(int volume);
    void dealChangeSoundVolume(int volume);
    void dealFinished();

private:
    Ui::MainWidget *ui;


public:
    StartWidget *start;
    SettingWidget *setting;
    OnlineWidget *online;
    LocationWidget *locate;
    MediaController *media;
    QSqlDatabase db;
};

#endif // MAINWIDGET_H
