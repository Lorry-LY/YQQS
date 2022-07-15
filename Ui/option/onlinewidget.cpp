#include "onlinewidget.h"
#include "ui_onlinewidget.h"

OnlineWidget::OnlineWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OnlineWidget)
{
    ui->setupUi(this);
}

OnlineWidget::~OnlineWidget()
{
    delete ui;
}
