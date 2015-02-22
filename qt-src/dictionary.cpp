#include "Dictionary.h"

#include <fstream>
#include <string>
#include <iostream>
#include <QTextStream>
#include "logger.h"
#include <QVector>
#include <ctime>
#include <QSettings>
#include <QDir>
#include <QDebug>
#include <QSqlError>

Dictionary::Dictionary(QObject *parent) :
    QObject(parent)
{

    loadDictionary();
}

QString Dictionary::getFirstWord(int width)
{
    srand(time(NULL));
    QVector<QString> firstWordList;
    QSqlQuery query(db);
    query.exec(GET_WORDS_QUERY);
    qDebug() << query.lastError();
    while (query.next()) {
        QString word = query.value(0).toString();
        if (word.length() == width) {
            firstWordList.push_back(word);
        }
    }

    QString firstWord = firstWordList.at((rand())%firstWordList.size());
    usedWords.insert(firstWord);
    return firstWord;
}

void Dictionary::loadDictionary() {

    std::cout<< QDir::currentPath().toStdString() << std::endl;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::currentPath() + "/dictionary.db");
    Logger l;
    if (!db.open()) {
        l.printLog(ERROR, "Database doesn't loaded");
    } else {
        l.printLog(ERROR, "Base loaded");
    }
}

void Dictionary::setUpConnection (QObject* wordCollector) {
    connect(this, SIGNAL(sendCheckResult(int)), wordCollector, SLOT(setWordApproved(int)));
}

void Dictionary::connectToBot(QObject *bot) {
    connect(this, SIGNAL(sendDictionary(QVector<QString>)), bot, SLOT(setupDictionary(QVector<QString>)));
}

//slots

void Dictionary::checkWord(const QString& word) {
    QSqlQuery query(db);
    query.prepare(CHECK_WORD_QUERY);
    query.bindValue(":word", word);
    if (!query.exec()) {
        exit(EXIT_FAILURE);
    }
    if (query.next()) {
        if (usedWords.find(word) == usedWords.end()) {
            usedWords.insert(word);
            emit sendCheckResult(1);
        } else {
            emit sendCheckResult(0);
        }
    } else {
        emit sendCheckResult(0);
    }
}


void Dictionary::sendDictionary() {
    Logger l;
    l.printLog(DEBUG, "Connected dictionary from bot");
    QVector<QString> words;
    QSqlQuery query(db);
    query.exec(GET_WORDS_QUERY);

    while (query.next()) {
        words.push_back(query.value(0).toString());
    }
    emit sendDictionary(words);
}
