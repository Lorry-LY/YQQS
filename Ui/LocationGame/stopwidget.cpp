#include "stopwidget.h"
#include "ui_stopwidget.h"

StopWidget::StopWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StopWidget)
{
    move(315,190);
    ui->setupUi(this);
}

StopWidget::~StopWidget()
{
    delete ui;
}

void StopWidget::on_PushButton_Continue_clicked()
{
    this->hide();
    emit Continue();
}

void StopWidget::on_PushButton_BackToHall_clicked()
{
    this->hide();
    emit BackToHall();
}

void StopWidget::on_PushButton_Setting_clicked()
{
    emit Setting();

}
