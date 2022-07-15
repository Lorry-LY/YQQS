#ifndef LOGLOCATIONWIDGET_H
#define LOGLOCATIONWIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTextEdit>
#include <QMessageBox>
#include <QCryptographicHash>

namespace Ui {
class LogLocationWidget;
}

class LogLocationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LogLocationWidget(QWidget *parent = nullptr);
    ~LogLocationWidget();

    QString getID();


private slots:


    void on_PushButton_Login_clicked();

    void on_PushButton_Reg_clicked();

    void on_PushButton_Back_clicked();

signals:
    void Login();
    void BackToStart();
    void Reg();

private:
    Ui::LogLocationWidget *ui;
    QSqlDatabase db;
    QString ID;
};

#endif // LOGLOCATIONWIDGET_H
