#include "locationwidget.h"
#include "ui_locationwidget.h"
#include <QDebug>

LocationWidget::LocationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LocationWidget)
{
    ui->setupUi(this);

    db = QSqlDatabase::database("QSQLITE");

    loglocation = new LogLocationWidget(this);
    loglocation->show();
    reg = new RegWidget(this);
    reg->hide();
    hall = new HallWidget(this);
    hall->hide();
    end = new EndWidget(this);
    end->hide();

    connect(loglocation,&LogLocationWidget::Login,this,&LocationWidget::dealLogin);
    connect(loglocation,&LogLocationWidget::Reg,this,&LocationWidget::dealReg);
    connect(reg,&RegWidget::BackToLogin,this,&LocationWidget::dealBackToLogin);
    connect(hall,&HallWidget::BackToLogin,this,&LocationWidget::dealBackToLogin);
    connect(hall,&HallWidget::GameContinue,this,[=](int number,int gameTime,int characterID,int HP,int MP,int coin,int weaponID){
        hall->hide();
        game = new GameWidget(this);
        connect(game->getStop(),SIGNAL(BackToHall()),this,SLOT(dealBackToHall()));
        connect(game,&GameWidget::die,this,&LocationWidget::dealEnd);
        connect(game,&GameWidget::saveGame,this,&LocationWidget::saveGame);
        game->continueGame(number,gameTime,characterID,HP,MP,coin,weaponID);
        game->show();
        game->setFocus();
    });
    connect(hall,SIGNAL(GameStart(int)),this,SLOT(dealGameStart(int)));
    connect(end,&EndWidget::BackToHall,this,&LocationWidget::dealEndToHall);

}

LocationWidget::~LocationWidget()
{
    delete ui;
}

void LocationWidget::saveGame()
{
    QVector<QString>gameInformation =  game->getInformation();
    QString ID = loglocation->getID();
    QString character,level,HP,MP,money,weapon,time;
    character = gameInformation[0];
    level = gameInformation[1];
    time = gameInformation[2];
    HP = gameInformation[3];
    MP = gameInformation[4];
    money = gameInformation[5];
    weapon = gameInformation[6];
    QSqlQuery query = QSqlQuery(db);
    query.prepare("delete from '状态记录' where 玩家ID= :ID");
    query.bindValue(":ID",ID.toInt());
    query.exec();
    query.prepare("insert into  状态记录 values(null,:id,:character,:level,:time,:HP,:MP,:money,:weapon)");
    query.bindValue(":id",ID.toInt());
    query.bindValue(":character",character.toInt());
    query.bindValue(":level",level.toInt());
    query.bindValue(":time",time.toInt());
    query.bindValue(":HP",HP.toInt());
    query.bindValue(":MP",MP.toInt());
    query.bindValue(":money",money.toInt());
    query.bindValue(":weapon",weapon.toInt());
    query.exec();
}

//登陆界面跳转大厅界面
void LocationWidget::dealLogin(){
    loglocation->hide();
    hall->resume(loglocation->getID());
    hall->show();
}
//登陆界面跳转注册界面
void LocationWidget::dealReg(){
    loglocation->hide();
    reg->show();
}

//返回登陆界面
void LocationWidget::dealBackToLogin(){
    reg->hide();
    hall->hide();
    loglocation->show();
}

//开始游戏
void LocationWidget::dealGameStart(int CharacterID){
    hall->hide();
    game = new GameWidget(this);
    connect(game->getStop(),SIGNAL(BackToHall()),this,SLOT(dealBackToHall()));
    //    connect(game,SIGNAL(End(QString,QString,QString,QString,QString)),this,SLOT(dealEnd(QString,QString,QString,QString,QString)));
    connect(game,&GameWidget::die,this,&LocationWidget::dealEnd);
    connect(game,&GameWidget::saveGame,this,&LocationWidget::saveGame);
    game->init(CharacterID);
    game->show();
    game->setFocus();

    QString ID = loglocation->getID();
    QSqlQuery query = QSqlQuery(db);
    query.prepare("delete from '状态记录' where 玩家ID= :ID");
    query.bindValue(":ID",ID.toInt());
    query.exec();
}

//退出游戏返回大厅
void LocationWidget::dealBackToHall(){
    game->close();
    hall->show();
    hall->resume(loglocation->getID());
}


//结算界面
void LocationWidget::dealEnd(int time,QPixmap pixmap,int GoldNum,int Level,QString Result,QString characterName,QString weaponName,int coin){
    game->close();
    end->init(time,pixmap,GoldNum,Level,Result);
    end->show();

    QString ID = loglocation->getID();
    QSqlQuery query = QSqlQuery(db);
    query.prepare("delete from '状态记录' where 玩家ID= :ID");
    query.bindValue(":ID",ID.toInt());
    query.exec();
    query.prepare(QString("insert into '游戏记录' values(null,'%1','%2','%3',%4,%5)").arg(ID).arg(characterName).arg(weaponName).arg(coin).arg(time));
    query.bindValue(":ID",ID.toInt());
    query.exec();

}

//结束界面返回到大厅
void LocationWidget::dealEndToHall()
{
    end->close();
    hall->resume(loglocation->getID());
    hall->show();
}
