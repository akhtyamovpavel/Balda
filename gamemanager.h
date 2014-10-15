#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QObject>
#include "board.h"
#include "Dictionary.h"
#include "Player.h"
#include "wordcollector.h"

class GameManager : public QObject
{
    Q_OBJECT
    Board board;
    Dictionary dict;
    WordCollector wc;
    Player player1;
    Player player2;
    int numberOfSpareCells;
    Player* currentPlayer;
    const int FIRST_PLAYER = 1;
    const int SECOND_PLAYER = 2;
    int currentID;

    bool isGameEnded();
public:
    explicit GameManager(QObject *parent = 0);
    void runGame();

signals:
    void startMove();
    void askForCells();

public slots:
    void stepEnded();
    void getNumberOfCells(int value);
};

#endif // GAMEMANAGER_H
