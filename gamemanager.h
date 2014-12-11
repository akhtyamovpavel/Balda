#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QObject>
#include "board.h"
#include "Dictionary.h"
#include "player.h"
#include "wordcollector.h"
#include "bot/bot.h"
#include <lang/language.h>

class GameManager : public QObject
{
    Q_OBJECT
    Board board;
    Dictionary dict;
    WordCollector wc;
    Player player1;
    Player player2;
    Bot bot;
    int numberOfSpareCells;
    QString firstWord;
    int playersNumber;
    Player* currentPlayer;
    const int FIRST_PLAYER = 1;
    const int SECOND_PLAYER = 2;
    int currentID;
    const int EASY = 14;
    const int HARD = 88;
    const int MEDIUM = 265;
    const int HARDEST = 228;
    int width;
    int height;

    bool isGameEnded();
public:
    explicit GameManager(Language *language,
                         int width,
                         int height,
                         int playersNumber,
                         QString level = "",
                         QObject *parent = 0);
    void runGame();
    Player* getFirstPlayer();
    Player* getSecondPlayer();
    int getCurrentPlayer();
    QString getFirstWord();


signals:
    void startMoveFirst();
    void startMoveSecond();
    void askForCells();
    void gameEnded(const QString& message);


    /*
     * Signals to Board
     * */

    void showBoard();

public slots:
    void stepEnded();
    void getNumberOfCells(int value);
    void gameEnding();
};

#endif // GAMEMANAGER_H
