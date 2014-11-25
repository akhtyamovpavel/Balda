#include <QApplication>
#include <iostream>
#include <QTextStream>
#include <QTextCodec>
#include "gamemanager.h"
#include <QInputDialog>
#include <QMessageLogger>
#include "logger.h"
#include "interface/graphicboard.h"
#include <QString>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    //GameManager gm;
    //gm.runGame();


    GraphicBoard gb;
    QObject::connect(&gb, SIGNAL(quit()), &a, SLOT(quit()));
    gb.show();
    gb.runStep();

    return a.exec();
}
