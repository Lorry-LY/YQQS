#include "hallwidget.h"
#include "ui_hallwidget.h"

HallWidget::HallWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HallWidget)
{
    ui->setupUi(this);
    db = QSqlDatabase::database("QSQLITE");
    init();



    tim = new QTimer();

    tim->setInterval(350);

    connect(tim,SIGNAL(timeout()),this,SLOT(onTimeOut()));

    tim->start();

    this->CharacterID = -1;



}

HallWidget::~HallWidget()
{
    delete ui;
}

void HallWidget::resume(QString ID)
{
    ui->PushButton_ResumeGame->hide();
    QSqlQuery query = QSqlQuery(db);
    query.prepare("select * from '状态记录' where 玩家ID= :ID");
    query.bindValue(":ID",ID.toInt());
    query.exec();
    this->playerID = ID;
    if (query.next()){

        ui->PushButton_ResumeGame->show();
    }
}


void HallWidget::init(){
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT 玩家ID,角色,武器,通关时长 FROM 游戏记录 ORDER BY 通关时长");
    model->setHeaderData(0, Qt::Horizontal, tr("玩家ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("角色"));
    model->setHeaderData(2, Qt::Horizontal, tr("武器"));
    model->setHeaderData(3, Qt::Horizontal, tr("游戏时长"));

    ui->PushButton_ResumeGame->hide();
    ui->tableView->setModel(model);
    ui->tableView->hide();
    ui->tableView->setColumnWidth(0,200);
    ui->tableView->setColumnWidth(1,100);
    ui->tableView->setColumnWidth(2,100);
    ui->tableView->setColumnWidth(3,100);


}


void HallWidget::on_PushButton_Ranking_clicked()
{
    if(tag0){
        ui->tableView->show();
        tag0=false;
    }
    else{
        ui->tableView->hide();
        tag0=true;
    }

}

void HallWidget::on_BackToLogin_clicked()
{
    choose = 1;
    emit BackToLogin();
}

void HallWidget::onTimeOut(){
    if (tag){
        if(choose == 1){
            ui->PushButton_knight->setStyleSheet("QPushButton{border-image:url(resource/image/character/Knight/stop1.png);}");
            ui->PushButton_Magician->setStyleSheet("QPushButton{border-image:url(resource/image/character/Magician/stop1.png);}");
            tag=false;
        }
        else if(choose == 2){
            ui->PushButton_knight->setStyleSheet("QPushButton{border-image:url(resource/image/character/Knight/stop1.png);}");
            ui->PushButton_Magician->setStyleSheet("QPushButton{border-image:url(resource/image/character/Magician/stop1.png);}");
            tag=false;
        }
        else if(choose == 3){
            ui->PushButton_knight->setStyleSheet("QPushButton{border-image:url(resource/image/character/Knight/stop1.png);}");
            ui->PushButton_Magician->setStyleSheet("QPushButton{border-image:url(resource/image/character/Magician/stop1.png);}");
            tag=false;
        }
    }
    else{

        if(choose == 1){
            ui->PushButton_knight->setStyleSheet("QPushButton{border-image:url(resource/image/character/Knight/stop2.png);}");
            ui->PushButton_Magician->setStyleSheet("QPushButton{border-image:url(resource/image/character/Magician/stop2.png);}");
            tag=true;
        }
        else if(choose == 2){
            ui->PushButton_knight->setStyleSheet("QPushButton{border-image:url(resource/image/character/Knight/stop1.png);}");
            ui->PushButton_Magician->setStyleSheet("QPushButton{border-image:url(resource/image/character/Magician/stop2.png);}");
            tag=true;
        }
        else if(choose == 3){
            ui->PushButton_knight->setStyleSheet("QPushButton{border-image:url(resource/image/character/Knight/stop2.png);}");

            ui->PushButton_Magician->setStyleSheet("QPushButton{border-image:url(resource/image/character/Magician/stop1.png);}");
            tag=true;
        }
    }
}

void HallWidget::on_PushButton_knight_clicked()
{
    CharacterID=1;
    choose = 2;
}

void HallWidget::on_PushButton_Magician_clicked()
{
    CharacterID=0;
    choose = 3;
}

void HallWidget::on_PushButton_StartGame_clicked()
{
    if (CharacterID==-1){
        QMessageBox::warning(this,"提示","请选择角色");
        return;
    }
    choose = 1;
    emit GameStart(CharacterID);
}

void HallWidget::on_PushButton_ResumeGame_clicked()
{
    QSqlQuery query = QSqlQuery(db);
    query.prepare("select * from '状态记录' where 玩家ID= :ID");
    query.bindValue(":ID",playerID.toInt());
    query.exec();
    if(query.next())
    {
        int number = query.value("关卡").toInt();
        int gameTime = query.value("游戏时间").toInt();
        int characterID = query.value("角色").toInt();
        int HP = query.value("生命值").toInt();
        int MP = query.value("能量").toInt();
        int coin = query.value("金币").toInt();
        int weaponID = query.value("武器").toInt();
        emit GameContinue(number,gameTime,characterID,HP,MP,coin,weaponID);
    }

}
