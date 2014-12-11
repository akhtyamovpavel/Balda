#-------------------------------------------------
#
# Project created by QtCreator 2014-11-14T14:41:25
#
#-------------------------------------------------

QT       += core gui sql

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
    interface/buttoncell.cpp \
    interface/graphicboard.cpp \
    external/settingsdialog.cpp \
    lang/language.cpp \
    lang/russianlang.cpp \
    lang/englishlang.cpp

HEADERS  += mainwindow.h \
    board.h \
    cell.h \
    player.h \
    wordcollector.h \
    bot/bor.h \
    bot/bot.h \
    bot/letter.h \
    logger.h \
    gamemanager.h \
    interface/buttoncell.h \
    interface/graphicboard.h \
    dictionary.h \
    external/settingsdialog.h \
    lang/language.h \
    lang/russianlang.h \
    lang/englishlang.h

FORMS    += mainwindow.ui
