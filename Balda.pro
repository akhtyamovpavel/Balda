#-------------------------------------------------
#
# Project created by QtCreator 2014-11-08T00:04:46
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = Balda
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    board.cpp \
    cell.cpp \
    dictionary.cpp \
    gamemanager.cpp \
    logger.cpp \
    player.cpp \
    tst_testmainclassestest.cpp \
    wordcollector.cpp \
    bot/bor.cpp \
    bot/bot.cpp \
    bot/letter.cpp

HEADERS += \
    board.h \
    cell.h \
    Dictionary.h \
    gamemanager.h \
    logger.h \
    player.h \
    wordcollector.h \
    bot/bor.h \
    bot/bot.h \
    bot/letter.h
