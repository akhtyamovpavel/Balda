#include <QCoreApplication>
#include <iostream>
#include <QTextStream>
#include "gamemanager.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    GameManager gm;
    gm.runGame();




    return a.exec();
}
