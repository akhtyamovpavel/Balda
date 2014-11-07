#include "gamemanager.h"
#include <iostream>
GameManager::GameManager(QObject *parent) :
    QObject(parent)
{
    wc.connectToDictionary(&dict);
    wc.connectToBoard(&board);
    dict.setUpConnection(&wc);
    board.setUpConnection(&wc);

    std::cout << "How many players\n";
    int playersNumber;
    std::cin>> playersNumber;

    if (playersNumber == 2) {
        player1.connectToBoard(&board);
        player1.connectToManager(this);
        player2.connectToBoard(&board);
        player2.connectToManager(this);
    }
    else {
        player1.connectToBoard(&board);
        player1.connectToManager(this);
        dict.connectToBot(&bot);
        bot.connectToBoard(&board);
        bot.connectToManager(this);
        bot.connectToDictionary(&dict);
    }


    currentPlayer = &player1;
    currentID = FIRST_PLAYER;
    numberOfSpareCells = 4;
    if (playersNumber == 2) {
        board.connectToPlayers(&player1, &player2);
    } else {
        board.connectToPlayers(&player1, &bot);
    }
    connect(this, SIGNAL(askForCells()), &board, SLOT(getNumberOfCells()));
    connect(this, SIGNAL(startMoveFirst()), &player1, SLOT(beginStep()));
    if (playersNumber == 2) {
        connect(this, SIGNAL(startMoveSecond()), &player2, SLOT(beginStep()));
    } else {
        connect(this, SIGNAL(startMoveSecond()), &bot, SLOT(beginStep()));
    }
    connect(this, SIGNAL(showBoard()), &board, SLOT(showBoardToManager()));
}

bool GameManager::isGameEnded() {
    emit askForCells();
    std::cout << numberOfSpareCells << std::endl;
    return (numberOfSpareCells == 0);
}


void GameManager::runGame() {
    while (!isGameEnded()) {
        emit showBoard();
        if (currentID == FIRST_PLAYER) {
            std::cout << "First player: your move" << std::endl;
            emit startMoveFirst();
        } else {
            std::cout << "Second player: your move" << std::endl;
            emit startMoveSecond();
        }
        --numberOfSpareCells;

    }
    int score1 = player1.getScore();
    int score2 = player2.getScore();
    if (score1 > score2) {
        std::cout << "First player wins" << std::endl;
    } else if (score1 == score2){
        std::cout << "Draw" << std::endl;
    } else {
        std::cout << "Second player wins" << std::endl;
    }
    std::cout << "Game over" << std::endl;
}
//Slots

void GameManager::stepEnded() {
    if (currentID == FIRST_PLAYER) {
        currentID = SECOND_PLAYER;
    } else {
        currentID = FIRST_PLAYER;
    }

}

void GameManager::getNumberOfCells(int value) {
    numberOfSpareCells = value;
}
