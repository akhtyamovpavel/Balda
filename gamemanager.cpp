#include "gamemanager.h"
#include <iostream>
GameManager::GameManager(
        Language *language,
        int width,
        int height,
        int playersNumber,
        QString level,
        QObject *parent) :
    width(width),
    height(height),
    QObject(parent),
    playersNumber(playersNumber),
    bot{language, width, height, this}
{
    board.initBoard(width, height);

    wc.connectToDictionary(&dict);
    wc.connectToBoard(&board);
    dict.setUpConnection(&wc);
    board.setUpConnection(&wc);

    firstWord = dict.getFirstWord(width);

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
        if (level == "EASY") {
            bot.setLevel(EASY);
        } else if (level == "MEDIUM") {
            bot.setLevel(MEDIUM);
        } else if (level == "HARD") {
            bot.setLevel(HARD);
        } else if (level == "HARDEST") {
            bot.setLevel(HARDEST);
        }
        bot.connectToBoard(&board);
        bot.connectToManager(this);
        bot.connectToDictionary(&dict);
    }


    currentPlayer = &player1;
    currentID = FIRST_PLAYER;
    numberOfSpareCells = width*(height - 1);
    if (playersNumber == 2) {
        board.connectToPlayers(&player1, &player2);
        board.setFirstPlayer(FIRST_PLAYER);
    } else {
        board.connectToPlayers(&player1, &bot);
        board.setFirstPlayer(FIRST_PLAYER);
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
    if (!isGameEnded()) {
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
            if (currentID == FIRST_PLAYER) {
                std::cout << "First player: your move" << std::endl;
                emit startMoveFirst();
            } else {
                std::cout << "Second player: your move" << std::endl;
                emit startMoveSecond();
            }
        }
        return;
    }
    gameEnding();
}

void GameManager::gameEnding()
{
    QString message;
    if (playersNumber == 2) {
        int score1 = player1.getScore();
        int score2 = player2.getScore();
        if (score1 > score2) {
            message = tr("First player won");
        } else if (score1 == score2){
            message = tr("Draw");
        } else {
            message = tr("Second player won");
        }
    } else {
        int score1 = player1.getScore();
        int score2 = bot.getScore();
        if (score1 > score2) {
            message = tr("Player won");
        } else if (score1 == score2){
            message = tr("Draw");
        } else {
            message = tr("Computer wins");
        }
    }
    emit gameEnded(message);
}

Player* GameManager::getFirstPlayer()
{
    return &player1;
}



Player* GameManager::getSecondPlayer()
{
    if (playersNumber == 2) {
        return &player2;
    } else {
        return &bot;
    }
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
