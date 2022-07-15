#include "endwidget.h"
#include "ui_endwidget.h"

EndWidget::EndWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EndWidget)
{
    ui->setupUi(this);

}

EndWidget::~EndWidget()
{
    delete ui;
}

void EndWidget::init(int time, QPixmap pixmap, int GoldNum, int Level, QString Result)
{
    int H = time / (60*60);
    int M = (time- (H * 60 * 60)) / 60;
    int S = (time - (H * 60 * 60)) - M * 60;
    QString hour = QString::number(H);
    if (hour.length() == 1) hour = "0" + hour;
    QString min = QString::number(M);
    if (min.length() == 1) min = "0" + min;
    QString sec = QString::number(S);
    if (sec.length() == 1) sec = "0" + sec;
    QString qTime = hour + ":" + min + ":" + sec;

    ui->Label_Gold->setText("金币数量: "+QString::number(GoldNum));
    ui->Label_Time->setText("时间: "+qTime);
    ui->Label_EnemyIcon->setPixmap(pixmap);
    ui->Label_Level->setText("关卡: "+QString::number(Level));
    ui->Label_Result->setText(Result);
}




void EndWidget::on_PushButton_BackToHall_clicked()
{
    emit BackToHall();
}

