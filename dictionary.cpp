#include "dictionary.h"
#include <fstream>
#include <string>
#include <iostream>
#include <QTextStream>

Dictionary::Dictionary(QObject *parent) :
    QObject(parent)
{
    loadDictionary();
}

void Dictionary::loadDictionary(){

    QFile file ("dictionary.txt");
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        QString word;
        while (!stream.atEnd())
        {
            word = stream.readLine();
            //QTextStream out(stdout);
            //out << word << tr("\n");
            setOfWords.insert(word);

        }
        if(stream.status()!= QTextStream::Ok)
        {
            std::cout << "Read file error";
        }
        file.close();
    }
    std::cout << "Dictionary loaded" << std::endl;

}

void Dictionary::setUpConnection (QObject* wordCollector) {
    connect(this, SIGNAL(sendCheckResult(int)), wordCollector, SLOT(setWordApproved(int)));
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
