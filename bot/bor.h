#ifndef BOR_H
#define BOR_H


#include <string>
#include <vector>
#include <utility>
#include <QChar>
#include <QString>
#include <QVector>
#include <QPair>
#include "letter.h"
const int NOT_FOUND = -1;
const int PRE_VERTEX = 0;



class Bor{

public:
    std::vector<Letter> borVertices;
    void addLetter(int position, QChar symbol);
    void addWord(const QString& word);
    bool notBelong(std::vector<QString> &notAllowedWords, QString &checkIn);
    Bor();
};

struct Word
{

    QString possibleWord;
    QVector<QPair<int, int> > coordinates;
    Word(){}

    Word(QString &pushString, const QVector<QPair<int, int> >& pushCoordinates)
    {
        possibleWord = pushString;
        coordinates = pushCoordinates;
    }
    bool operator < (const Word& w2)
    {
        return (possibleWord.length() < w2.possibleWord.length());
    }
};





#endif // BOR_H
