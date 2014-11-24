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


/*
 *This is the simplest realization of Directory and it doesn't read dictionary from file or database
 **/
class Dictionary : public QObject
{
    Q_OBJECT
    std::set<QString> setOfWords;
    std::set<QString> usedWords;
    void loadDictionary();
public:
    explicit Dictionary(QObject *parent = 0);

    QString getFirstWord();


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
