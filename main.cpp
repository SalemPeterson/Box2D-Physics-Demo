#include "mainwindow.h"
#include "worldmodel.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WorldModel m;
    MainWindow w{&m};
    w.show();
    return a.exec();
}
