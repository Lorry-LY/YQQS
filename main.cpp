
#include "Ui/mainwidget.h"
#include "Ui/LocationGame/gamewidget.h"

#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget w;
    w.show();
    return a.exec();
}
