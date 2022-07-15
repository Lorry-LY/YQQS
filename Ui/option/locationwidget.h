#ifndef LOCATIONWIDGET_H
#define LOCATIONWIDGET_H

#include <QWidget>
#include <QMessageBox>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "../LocationGame/endwidget.h"
#include "../LocationGame/gamewidget.h"
#include "../LocationGame/hallwidget.h"
#include "../LocationGame/loglocationwidget.h"
#include "../LocationGame/stopwidget.h"
#include "../LocationGame/regwidget.h"
#include "./startwidget.h"



namespace Ui {
class LocationWidget;
}

class LocationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LocationWidget(QWidget *parent = nullptr);
    ~LocationWidget();
    LogLocationWidget *loglocation;
private:
    Ui::LocationWidget *ui;
    QSqlDatabase db;
    HallWidget *hall;
    StartWidget *start;
    RegWidget *reg;
    GameWidget *game;
    StopWidget *stop;
    EndWidget *end;
    void saveGame();



signals:
    void back();



public slots:
    void dealGameStart(int CharacterID);
    void dealReg();
    void dealLogin();
    void dealBackToLogin();
    void dealBackToHall();
    void dealEnd(int time,QPixmap pixmap,int GoldNum,int Level,QString Result,QString characterName,QString weaponName,int coin);
    void dealEndToHall();

};

#endif // LOCATIONWIDGET_H
