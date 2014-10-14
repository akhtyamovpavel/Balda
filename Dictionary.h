#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <set>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <iostream>
#include <QObject>


/*
 *This is the simplest realization of Directory and it doesn't read dictionary from file or database
 **/
class Dictionary : public QObject
{
    Q_OBJECT
    std::set<QString> setOfWords;
    void loadDictionary();
public:
    explicit Dictionary(QObject *parent = 0);


    void setUpConnection (QObject* wordCollector);


signals:
    void sendCheckResult(int value);

public slots:
    void checkWord(const QString& word);
};


#endif // DICTIONARY_H
