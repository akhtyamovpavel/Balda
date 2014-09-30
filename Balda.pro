#-------------------------------------------------
#
# Project created by QtCreator 2014-09-28T17:35:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Balda
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Table.cpp

HEADERS  += mainwindow.h \
    GameManager.h \
    Cell.h \
    Table.h \
    GameDataStorage.h \
    Dictionary.h \
    LetterMenu.h

FORMS    += mainwindow.ui
