#include "loglocationwidget.h"
#include "ui_loglocationwidget.h"

LogLocationWidget::LogLocationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogLocationWidget)
{
    ui->setupUi(this);

    setWindowTitle("登录");
    db = QSqlDatabase::database();

    ui->LineEdit_LPassword->setEchoMode(QLineEdit::Password);


}

LogLocationWidget::~LogLocationWidget()
{
    delete ui;
}

QString LogLocationWidget::getID()
{
    return ID;
}



void LogLocationWidget::on_PushButton_Login_clicked()
{

    QString LID = ui->LineEdit_LID->text().toUtf8().data();
    QString LPassword = ui->LineEdit_LPassword->text().toUtf8().data();

    //    判断账号是否为空
    if(LID==""){
        QMessageBox::warning(this,"Title","输入账号为空!");
        return;
    }
    //    判断密码是否为空
    if(LPassword==""){
        QMessageBox::warning(this,"Title","输入密码为空!");
        return;
    }
    //    调用数据库
    QSqlQuery query=QSqlQuery(db);
    query.exec("select * from 玩家");

    while(query.next()){
        QString q_ID=query.value(0).toString();
        QString q_Password=query.value(2).toString();
        QByteArray temp=QCryptographicHash::hash(LPassword.toLatin1(),QCryptographicHash::Md5);//密码采用md5加密
        QString password=QString(temp.toHex());
        if(q_ID==LID&&q_Password==password){
            ID =  ui->LineEdit_LID->text();
            ui->LineEdit_LID->clear();
            ui->LineEdit_LPassword->clear();
            emit Login();
            return;
        }
    }
    QMessageBox::warning(this,"提示","用户名或密码错误");
}

void LogLocationWidget::on_PushButton_Reg_clicked()
{
    ui->LineEdit_LID->clear();
    ui->LineEdit_LPassword->clear();
    emit Reg();
}

void LogLocationWidget::on_PushButton_Back_clicked()
{
    ui->LineEdit_LID->clear();
    ui->LineEdit_LPassword->clear();
    emit BackToStart();
}
