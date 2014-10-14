#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QTextStream>
#include <vector>
#include <QPair>
#include "cell.h"

class Board : public QObject
{
    Q_OBJECT
    std::vector<std::vector<Cell*> > board_;
    bool isChanged;

public:
    explicit Board(QObject* wordCollector, QObject *parent = 0);

    void setUpConnection(QObject* wordCollector);

    void showBoard(QTextStream& out);

    void changeLetter(int x, int y, QChar letter);

    void pushLetter(int x, int y);

    bool isMarked(int x, int y);

    void setMarked(int x, int y, bool value);

    QChar getLetter(int x, int y);

    void setLetter(int x, int y, QChar letter);

    bool hasChanged();

    void setChanged(bool changed);




signals:
    void commitLetter(int value);
    void commitX(int x);
    void commitY(int y);
    void commitWord();
    void commitNew(int flag);

public slots:
    void resetState(const QPair<int,int>& coordinates);
    void remakeMove();
};

#endif // BOARD_H
