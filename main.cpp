#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("fusion");
    mainWindow w;
    w.move(1500, 50); //1920-400-20
    w.show();
    return a.exec();
}
