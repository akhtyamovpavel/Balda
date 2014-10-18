#include <QCoreApplication>
#include <iostream>
#include <QTextStream>
#include <QTextCodec>
#include "gamemanager.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("cp866"));
    GameManager gm;
    gm.runGame();

    return a.exec();
}
