#include "logindialog.hpp"
#include <QSqlDatabase>

#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    LoginDialog w;
    w.show();
    return a.exec();
}
