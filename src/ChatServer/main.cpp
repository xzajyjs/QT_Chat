#include "serverdialog.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ServerDialog w;
    w.show();
    return a.exec();
}
