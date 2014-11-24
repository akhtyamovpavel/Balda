#-------------------------------------------------
#
# Project created by QtCreator 2014-11-14T14:41:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Balda
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    board.cpp \
    cell.cpp \
    dictionary.cpp \
    gamemanager.cpp \
    player.cpp \
    tst_testmainclassestest.cpp \
    wordcollector.cpp \
    bot/bor.cpp \
    bot/bot.cpp \
    bot/letter.cpp \
    logger.cpp \
    interface/buttoncell.cpp \
    interface/graphicboard.cpp

HEADERS  += mainwindow.h \
    board.h \
    cell.h \
    Dictionary.h \
    player.h \
    wordcollector.h \
    bot/bor.h \
    bot/bot.h \
    bot/letter.h \
    logger.h \
    gamemanager.h \
    interface/buttoncell.h \
    interface/graphicboard.h

FORMS    += mainwindow.ui
