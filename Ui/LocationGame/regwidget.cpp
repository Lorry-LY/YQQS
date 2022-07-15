#include "regwidget.h"
#include "ui_regwidget.h"

RegWidget::RegWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegWidget)
{
    ui->setupUi(this);

    //设置标题
    setWindowTitle("注册");

    db = QSqlDatabase::database();

    //设置密码输入框样式
    ui->LineEdit_RPassword->setEchoMode(QLineEdit::Password);
    ui->LineEdit_RPasswordAck->setEchoMode(QLineEdit::Password);




}

RegWidget::~RegWidget()
{
    db.close();
    delete ui;
}



void RegWidget::on_PushButton_RegAck_clicked()
{
    QString RID = ui->LineEdit_RID->text().toUtf8().data();
    QString RPassword = ui->LineEdit_RPassword->text().toUtf8().data();
    QString RPasswordACK = ui->LineEdit_RPasswordAck->text().toUtf8().data();
    QString Name = ui->LineEdit_Name->text().toUtf8().data();


//    判断账号是否为空
    if(RID==""||RID.length()>20){
        QMessageBox::warning(this,"Title","输入账号为空!");
        return;
    }
//    判断密码是否为空
    if(RPassword==""){
        QMessageBox::warning(this,"Title","输入密码为空!");
        return;
    }
//    判断确认密码是否为空
    if(RPassword==""){
        QMessageBox::warning(this,"Title","输入确认密码为空!");
        return;
    }

//    判断密码是否相同
    if(RPassword!=RPasswordACK){
        QMessageBox::warning(this,"Title","两次密码不一致，请重新输入!");
        return;
    }

    QByteArray temp=QCryptographicHash::hash(RPassword.toLatin1(),QCryptographicHash::Md5);//密码采用md5加密
    QString password=QString(temp.toHex());



//    调用数据库
    QSqlQuery query=QSqlQuery(db);
    query.exec("select * from 玩家");

    while(query.next())
    {
        //qDebug()<<"数据库找到数据。";
        QString q_ID=query.value(0).toString();
        QString q_Name=query.value(1).toString();
        //判断账号是否被注册
        if(RID==q_ID)
        {
            QMessageBox::warning(this,"Title","该账号已被注册，请重新输入!");
            return;
        }
        //判断昵称是否被注册
        if(Name==q_Name){
            QMessageBox::warning(this,"Title","该昵称已被注册，请重新输入!");
            return;
        }
    }
    //把账号写入数据库
    query.prepare("insert into 玩家(玩家ID,昵称,密码) values(:number,:name,:password)");
    query.bindValue(":number",RID);
    query.bindValue(":name",Name);
    query.bindValue(":password",password);
    QMessageBox::warning(this,"成功","账号注册成功，请牢记账号和密码（无法找回）！");
    query.exec();


    ui->LineEdit_RID->clear();
    ui->LineEdit_Name->clear();
    ui->LineEdit_RPassword->clear();
    ui->LineEdit_RPasswordAck->clear();
    emit BackToLogin();
}


void RegWidget::on_PushButton_BackToLogin_clicked()
{
    ui->LineEdit_RID->clear();
    ui->LineEdit_Name->clear();
    ui->LineEdit_RPassword->clear();
    ui->LineEdit_RPasswordAck->clear();
    emit BackToLogin();
}

