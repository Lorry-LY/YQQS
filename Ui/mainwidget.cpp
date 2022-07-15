#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QDebug>
#include <QSqlError>


MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("YQQS.db");
    if(!db.open()){
          qDebug()<<db.lastError();
    }
    media = new MediaController(this);
    media->setObjectName("media");

    start = new StartWidget(this);
    start->show();

    setting = new SettingWidget(this);
    connect(setting,&SettingWidget::changeBGMvolume,this,&MainWidget::dealChangeBGMvolume);
    connect(setting,&SettingWidget::changeSoundVolume,this,&MainWidget::dealChangeSoundVolume);
    setting->init();
    setting->hide();
    online = new OnlineWidget(this);
    online->hide();
    locate = new LocationWidget(this);
    locate->hide();


    connect(start,&StartWidget::setting,this,&MainWidget::dealSetting);
    connect(start,&StartWidget::location,this,&MainWidget::dealLocation);
    connect(start,&StartWidget::online,this,&MainWidget::dealOnline);
    connect(setting,&SettingWidget::finished,this,&MainWidget::dealFinished);
    connect(locate->loglocation,&LogLocationWidget::BackToStart,this,&MainWidget::dealBack);
    this->setWindowTitle("元气骑士");
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::dealBack()
{
    locate->close();
    start->show();
}

void MainWidget::dealSetting()
{
    start->close();
    setting->init();
    setting->show();
}

void MainWidget::dealLocation(){

    start->close();
    locate->show();
}

void MainWidget::dealOnline(){
    start->close();
    online->show();
}

void MainWidget::dealChangeBGMvolume(int volume)
{
   media->setBGMVolume(volume);
}

void MainWidget::dealChangeSoundVolume(int volume)
{
   media->setSoundVolume(volume);
}


void MainWidget::dealFinished(){
    setting->close();
    start->show();
}

