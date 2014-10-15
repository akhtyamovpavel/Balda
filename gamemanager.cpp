#include "gamemanager.h"
#include <iostream>
GameManager::GameManager(QObject *parent) :
    QObject(parent)
{
    wc.connectToDictionary(&dict);
    wc.connectToBoard(&board);
    dict.setUpConnection(&wc);
    board.setUpConnection(&wc);
    player1.connectToBoard(&board);
    player1.connectToManager(this);

    currentPlayer = &player1;
    currentID = FIRST_PLAYER;
    numberOfSpareCells = 25;
    board.connectToPlayers(&player1, &player2);
    connect(this, SIGNAL(askForCells()), &board, SLOT(getNumberOfCells()));
    connect(this, SIGNAL(startMove()), &player1, SLOT(beginStep()));
}

bool GameManager::isGameEnded() {
    emit askForCells();
    std::cout << numberOfSpareCells << std::endl;
    return (numberOfSpareCells == 0);
}


void GameManager::runGame() {
    while (!isGameEnded()) {
        emit startMove();
        --numberOfSpareCells;
        //needs QSignalMapper to solve this problem
    }
}
//Slots

void GameManager::stepEnded() {
    if (currentID == FIRST_PLAYER) {
        //switch to the other player
    } else {

    }

}

void GameManager::getNumberOfCells(int value) {
    numberOfSpareCells = value;
}
