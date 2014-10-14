#include "dictionary.h"

Dictionary::Dictionary(QObject *parent) :
    QObject(parent)
{
    loadDictionary();
}

void Dictionary::loadDictionary(){


    setOfWords.insert("a");
    setOfWords.insert("b");
    setOfWords.insert("c");
}

void Dictionary::setUpConnection (QObject* wordCollector) {
    connect(this, SIGNAL(sendCheckResult(int)), wordCollector, SLOT(setWordApproved(int)));
}

//slots

void Dictionary::checkWord(const QString& word) {
    if (setOfWords.find(word) != setOfWords.end()) {
        emit sendCheckResult(1);
    } else {
        emit sendCheckResult(0);
    }
}
