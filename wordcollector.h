#ifndef WORDCOLLECTOR_H
#define WORDCOLLECTOR_H

#include <QObject>
#include <vector>
#include <QString>
#include "board.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <QPair>


class WordCollector : public QObject
{
    QString word_;
    std::vector<int> xList;
    std::vector<int> yList;
    std::vector<int> isNew;
    bool isApproved;

    Q_OBJECT

    void endMove();
public:
    explicit WordCollector(QObject *parent = 0);


    void connectToDictionary(QObject* dictionary);

    void connectToBoard(QObject* board);

    void clearWord();

signals:
    void sendToDictionary(const QString& word);
    void clearState(const QPair<int, int>& coordinates);
    void endOfTransaction(const QString& word);
    void approveWord();


public slots:
    void addLetter(int c);

    void addX(int x);

    void addY(int y);

    int hasApproved();

    void addNew(int isNewValue);

    void checkWord();

    void setWordApproved(int value);


};

#endif // WORDCOLLECTOR_H
