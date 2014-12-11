#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <set>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <iostream>
#include <QObject>
#include <QVector>

#include "logger.h"

#include <QSqlDatabase>
#include <QSqlQuery>

/*
 *This is the simplest realization of Directory and it doesn't read dictionary from file or database
 **/
class Dictionary : public QObject
{



    Q_OBJECT
    std::set<QString> setOfWords;
    std::set<QString> usedWords;
    QSqlDatabase db;
    const char* GET_WORDS_QUERY = "SELECT word FROM Words WHERE id = root_id";
    const char* TEST_QUERY = "SELECT 1";
    const char* CHECK_WORD_QUERY = "SELECT word FROM Words WHERE (id = root_id AND :word = word)";
    void loadDictionary();
public:
    explicit Dictionary(QObject *parent = 0);

    QString getFirstWord(int width);


    void setUpConnection(QObject* wordCollector);
    void connectToBot(QObject* bot);

signals:
    void sendCheckResult(int value);
    void sendDictionary(QVector<QString> words);

public slots:
    void checkWord(const QString& word);
    void sendDictionary();
};


#endif // DICTIONARY_H
