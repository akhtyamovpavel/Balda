#include <QCoreApplication>
#include <iostream>
#include <QTextStream>
#include <QTextCodec>
#include "gamemanager.h"

#include "logger.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("cp866"));
    Logger l;
    l.printLog(DEBUG, QChar('б').unicode());
    GameManager gm;
    gm.runGame();

    return a.exec();
}
