#include "settingwidget.h"
#include "ui_settingwidget.h"
#include <QDebug>
#include <QLabel>
#include <QMessageBox>

SettingWidget::SettingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingWidget)
{
    ui->setupUi(this);

    db = QSqlDatabase::database();
    connect(ui->BGMSlider,&QSlider::valueChanged,[=](int volumn){
        emit changeBGMvolume(volumn);
    });
    connect(ui->soundSlider,&QSlider::valueChanged,[=](int volumn){
        emit changeSoundVolume(volumn);
    });

    QList<QTextEdit*> texts = this->findChildren<QTextEdit*>();
    for(int i=0;i<texts.size();i++){
        texts[i]->setStyleSheet("color:white;background-color:rgb(121,87,60)");
        connect(texts[i],&QTextEdit::textChanged,[=](){
            QString textContent = texts[i]->toPlainText();
            int length = textContent.count();
            int maxLength = 1; // 最大字符数
            if(length > maxLength)
            {
                int position = texts[i]->textCursor().position();
                QTextCursor textCursor = texts[i]->textCursor();
                textContent.remove(position - (length - maxLength), length - maxLength);
                texts[i]->setText(textContent);
                textCursor.setPosition(position - (length - maxLength));
                texts[i]->setTextCursor(textCursor);
            }
            for(int j=0;j<texts.size();j++){
                if(j==i) continue;
                if(texts[j]->toPlainText() == texts[i]->toPlainText()) {
                    QMessageBox::information(this, "tips", "按键不允许相同", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                    texts[i]->setText("");
                }
            }
        });

    }
     ui->Save->setStyleSheet("background-color: rgb(121,87,60); color:white;");
     ui->Back->setStyleSheet("background-color: rgb(121,87,60); color:white;");
}



SettingWidget::~SettingWidget()
{
    delete ui;
}

void SettingWidget::init()
{

    QSqlQuery query = QSqlQuery(db);
    if(query.exec("select * from '设置'")){
        query.next();
        int bgm = query.value(0).toInt();
        ui->BGMSlider->setValue(bgm);
        emit changeBGMvolume(bgm);
        int sound = query.value(1).toInt();
        ui->soundSlider->setValue(sound);
        emit changeSoundVolume(sound);
        QString upmove = query.value(2).toString();
        ui->upMoveText->setText(upmove.toUpper());
        keys["上移"]=upmove;
        QString downmove = query.value(3).toString();
        ui->downMoveText->setText(downmove.toUpper());
        keys["下移"]=downmove;
        QString leftmove = query.value(4).toString();
        ui->leftMoveText->setText(leftmove.toUpper());
        keys["左移"]=leftmove;
        QString rightmove = query.value(5).toString();
        ui->rightMoveText->setText(rightmove.toUpper());
        keys["右移"]=rightmove;
        QString pick = query.value(6).toString();
        ui->pickText->setText(pick.toUpper());
        keys["拾取"]=pick;
        QString skill = query.value(7).toString();
        ui->skillText->setText(skill.toUpper());
        keys["技能"]=skill;
        QString buy = query.value(8).toString();
        ui->buyText->setText(buy.toUpper());
        keys["购买"]=buy;
        QString in = query.value(9).toString();
        ui->inPartolText->setText(in.toUpper());
        keys["进入"]=in;
    }
}







void SettingWidget::on_Save_clicked()
{

    int newBgm = ui->BGMSlider->value();
    int newSound = ui->soundSlider->value();
    QString newUpMove = ui->upMoveText->toPlainText();
    QString newDownMove = ui->downMoveText->toPlainText();
    QString newLeftMove = ui->leftMoveText->toPlainText();
    QString newRightMove = ui->rightMoveText->toPlainText();
    QString newPick= ui->pickText->toPlainText();
    QString newSkill= ui->skillText->toPlainText();
    QString newBuy = ui->buyText->toPlainText();
    QString newIn = ui->inPartolText->toPlainText();
    if(newUpMove=="" || newDownMove=="" || newLeftMove=="" || newRightMove=="" || newPick=="" || newSkill=="" || newBuy=="" || newIn==""){
        QMessageBox::information(this, "tips", "按键不允许为空", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    else{
        keys["上移"]=newUpMove;
        keys["下移"]=newDownMove;
        keys["左移"]=newLeftMove;
        keys["右移"]=newRightMove;
        keys["拾取"]=newPick;
        keys["技能"]=newSkill;
        keys["购买"]=newBuy;
        keys["进入"]=newIn;
        QString str1 = QString("update '设置' set upMove='%1', downMove='%2' , bgm='%3' ").arg(newUpMove).arg(newDownMove).arg(newBgm);
        QString str2 = QString("update '设置' set sound='%1',leftMove='%2' , rightMove='%3' ").arg(newSound).arg(newLeftMove).arg(newRightMove);
        QString str3 = QString("update '设置' set pick='%1',skill='%2' , buy='%3' , enter='%4'").arg(newPick).arg(newSkill).arg(newBuy).arg(newIn);
        QSqlQuery query = QSqlQuery(db);
        query.exec(str1);
        query.exec(str2);
        query.exec(str3);
        emit finished();
    }
}



void SettingWidget::on_Back_clicked()
{
    init();
    emit finished();
}
