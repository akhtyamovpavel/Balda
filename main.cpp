#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Table table(5,5);
    table.show();

    return a.exec();
}
