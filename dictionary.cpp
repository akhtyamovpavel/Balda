#include "dictionary.h"

#include <fstream>
#include <string>
#include <iostream>
#include <QTextStream>
#include "logger.h"
#include <QMessageLogger>
Dictionary::Dictionary(QObject *parent) :
    QObject(parent)
{
    loadDictionary();
}

void Dictionary::loadDictionary(){

    QMessageLogger l;
    QFile file("dictionary.txt");
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        QString word;
        while (!stream.atEnd())
        {
            word = stream.readLine();
            setOfWords.insert(word);

        }
        if(stream.status()!= QTextStream::Ok)
        {
            std::cout << "Read file error";
        }
        file.close();
    }
    l.debug("Dictionary loaded");
    std::cout << "Dictionary loaded" << std::endl;

}

void Dictionary::setUpConnection (QObject* wordCollector) {
    connect(this, SIGNAL(sendCheckResult(int)), wordCollector, SLOT(setWordApproved(int)));
}

void Dictionary::connectToBot(QObject *bot) {
    connect(this, SIGNAL(sendDictionary(QVector<QString>)), bot, SLOT(setupDictionary(QVector<QString>)));
}

//slots

void Dictionary::checkWord(const QString& word) {
    if (setOfWords.find(word) != setOfWords.end()) {
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
    for (std::set<QString>::iterator it = setOfWords.begin(); it != setOfWords.end(); ++it) {
        words.push_back(*it);
    }
    emit sendDictionary(words);
}
