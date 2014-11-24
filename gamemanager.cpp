#include "gamemanager.h"
#include <iostream>
GameManager::GameManager(int playersNumber, QObject *parent) :
    QObject(parent), playersNumber(playersNumber)
{
    wc.connectToDictionary(&dict);
    wc.connectToBoard(&board);
    dict.setUpConnection(&wc);
    board.setUpConnection(&wc);

    firstWord = dict.getFirstWord();

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
        std::cout << "What level do you choose\n";
        QString level;
        QTextStream in(stdin);
        in >> level;
        level.toLower();
        if (level == tr("easy")) {
            bot.setLevel(EASY);
        } else if (level == tr("hard")) {
            bot.setLevel(HARD);
        } else if (level == tr("hardest")) {
            bot.setLevel(HARDEST);
        }
        bot.connectToBoard(&board);
        bot.connectToManager(this);
        bot.connectToDictionary(&dict);
    }


    currentPlayer = &player1;
    currentID = FIRST_PLAYER;
    numberOfSpareCells = 20;
    if (playersNumber == 2) {
        board.connectToPlayers(&player1, &player2);
        board.setFirstPlayer(FIRST_PLAYER);
    } else {
        board.connectToPlayers(&player1, &bot);
        board.setFirstPlayer(SECOND_PLAYER);
    }
    connect(this, SIGNAL(askForCells()), &board, SLOT(getNumberOfCells()));
    connect(this, SIGNAL(startMoveFirst()), &player1, SLOT(beginStep()));
    if (playersNumber == 2) {
        connect(this, SIGNAL(startMoveSecond()), &player2, SLOT(beginStep()));
    } else {
        connect(this, SIGNAL(startMoveSecond()), &bot, SLOT(beginStep()));
    }
    board.setFirstWord(firstWord);
    //connect(this, SIGNAL(showBoard()), &board, SLOT(showBoardToManager()));
}

bool GameManager::isGameEnded() {
    emit askForCells();
    std::cout << numberOfSpareCells << std::endl;
    return (numberOfSpareCells == 0);
}


void GameManager::runGame() {
    while (!isGameEnded()) {
        emit showBoard();
        if (playersNumber == 2) {
            if (currentID == FIRST_PLAYER) {
                std::cout << "First player: your move" << std::endl;
                emit startMoveFirst();
            } else {
                std::cout << "Second player: your move" << std::endl;
                emit startMoveSecond();
            }
        }
        if (playersNumber == 1) {
            if (currentID == SECOND_PLAYER) {
                std::cout << "First player: your move" << std::endl;
                emit startMoveFirst();
            } else {
                std::cout << "Second player: your move" << std::endl;
                emit startMoveSecond();
            }
        }

    }

    if (playersNumber == 2) {
        int score1 = player1.getScore();
        int score2 = player2.getScore();
        if (score1 > score2) {
            std::cout << "First player wins" << std::endl;
        } else if (score1 == score2){
            std::cout << "Draw" << std::endl;
        } else {
            std::cout << "Second player wins" << std::endl;
        }
    } else {
        int score1 = player1.getScore();
        int score2 = bot.getScore();
        if (score1 > score2) {
            std::cout << "Player wins" << std::endl;
        } else if (score1 == score2){
            std::cout << "Draw" << std::endl;
        } else {
            std::cout << "Compucter wins" << std::endl;
        }
    }
    std::cout << "Game over" << std::endl;
}

Player* GameManager::getFirstPlayer()
{
    return &player1;
}

Player* GameManager::getSecondPlayer()
{
    return &player2;
}

int GameManager::getCurrentPlayer() {
    return currentID;
}

QString GameManager::getFirstWord()
{
    return firstWord;
}
//Slots

void GameManager::stepEnded() {
    if (currentID == FIRST_PLAYER) {
        currentID = SECOND_PLAYER;
    } else {
        currentID = FIRST_PLAYER;

    }
    --numberOfSpareCells;

}

void GameManager::getNumberOfCells(int value) {
    numberOfSpareCells = value;
}
