#ifndef REGWIDGET_H
#define REGWIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTextEdit>
#include <QInputDialog>
#include <QMessageBox>
#include <QCryptographicHash>

namespace Ui {
class RegWidget;
}

class RegWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RegWidget(QWidget *parent = nullptr);
    ~RegWidget();

private slots:  
    void on_PushButton_RegAck_clicked();

    void on_PushButton_BackToLogin_clicked();

signals:
    void RegAck();
    void BackToLogin();


private:
    Ui::RegWidget *ui;
    QSqlDatabase db;
};

#endif // REGWIDGET_H
