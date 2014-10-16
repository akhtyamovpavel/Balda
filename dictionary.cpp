#include "dictionary.h"
#include <fstream>
#include <string>
#include <iostream>

Dictionary::Dictionary(QObject *parent) :
    QObject(parent)
{
    loadDictionary();
}

void Dictionary::loadDictionary(){


    std::fstream input("dictionary.txt", std::fstream::in);
    if (!input.is_open()) {
        std::cout << "No library found" << std::endl;
    }
    std::string s;
    while(std::getline(input, s)) {
        QString currentWord(s.c_str());
        setOfWords.insert(currentWord);
    }
    input.close();
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
