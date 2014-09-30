#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <set>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <iostream>

class Dictionary{
    std::set<QString> setOfWords;

public:
    void loadDictionary(){
        QFile file("library.txt");
        if(!file.open(QIODevice::ReadOnly)){
            std::cerr << file.error() << std::endl;
        }

        QTextStream in(&file);
        while(!in.atEnd()){
            setOfWords.insert(in.readLine());
        }
        file.close();
    }

    QBool checkWord(const QString& word){
        return setOfWords.find(word) != setOfWords.end();

    }
}

#endif // DICTIONARY_H
