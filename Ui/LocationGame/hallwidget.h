#ifndef HALLWIDGET_H
#define HALLWIDGET_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTimer>
#include <QDebug>
#include <QMovie>
#include <QMessageBox>

#include "loglocationwidget.h"

namespace Ui {
class HallWidget;
}

class HallWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HallWidget(QWidget *parent = nullptr);
    ~HallWidget();
    void resume(QString ID);


private slots:
    void on_PushButton_Ranking_clicked();
    void on_BackToLogin_clicked();
    void onTimeOut();


    void on_PushButton_knight_clicked();

    void on_PushButton_Magician_clicked();

    void on_PushButton_StartGame_clicked();

    void on_PushButton_ResumeGame_clicked();

public: signals:
    void BackToLogin();
    void Ranking();
    void GameStart(int CharacterID);
    void GameContinue(int number,int gameTime,int characterID,int HP,int MP,int coin,int weaponID);

private:
    Ui::HallWidget *ui;
    void init();
    bool tag=true;
    bool tag0=true;
    int choose = 1;
    int CharacterID=0;
    QTimer *tim;
    QString playerID;
    QSqlDatabase db;
};

#endif // HALLWIDGET_H
