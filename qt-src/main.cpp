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
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    MainWindow w;
    w.show();

    return a.exec();
}
