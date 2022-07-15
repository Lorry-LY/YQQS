#include "startwidget.h"
#include "ui_startwidget.h"

StartWidget::StartWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartWidget)
{
    ui->setupUi(this);
    ui->Location->setStyleSheet("background-color: rgb(121,87,60); color:white;");
    ui->Online->setStyleSheet("background-color: rgb(121,87,60); color:white;");
    ui->setting->setStyleSheet("background-color: rgb(121,87,60); color:white;");

}

StartWidget::~StartWidget()
{
    delete ui;
}

void StartWidget::on_setting_clicked()
{
    emit setting();
}

void StartWidget::on_Location_clicked()
{
    emit location();
}

void StartWidget::on_Online_clicked()
{
    emit online();
}
