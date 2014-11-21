#include "board.h"
#include <iostream>
#include <QTextStream>
#include <QVector>

#include "logger.h"
#include "wordcollector.h"

Board::Board(QObject *parent) :
    QObject(parent)
{
    isChanged = false;
    isApproved = false;
    board_.resize(HEIGHT, std::vector<Cell*>(WIDTH));
    for (int i = 0; i < 5; ++i){
        for(int j = 0; j <5; ++j) {
            board_[i][j] = new Cell(QChar('-'), parent);
        }
    }
    currentPlayer = FIRST_PLAYER;

}


//Connections

void Board::setUpConnection(WordCollector* wordCollector) {
    connect(this, &Board::commitLetter, wordCollector, &WordCollector::addLetter);
    //connect(this, SIGNAL(commitLetter(QChar)), wordCollector, SLOT(addLetter(QChar)));
    connect(this, SIGNAL(commitX(int)), wordCollector, SLOT(addX(int)));
    connect(this, SIGNAL(commitY(int)), wordCollector, SLOT(addY(int)));
    connect(this, SIGNAL(commitWord()), wordCollector, SLOT(checkWord()));
    connect(this, SIGNAL(commitNew(int)), wordCollector, SLOT(addNew(int)));
    connect(this, SIGNAL(addNewLetter(QPair<int,int>)), wordCollector, SLOT(addChangedCell(QPair<int,int>)));
}

void Board::connectToPlayers(QObject* player1, QObject* player2) {
    connect(this, SIGNAL(moveEnded(QString)), player1, SLOT(approveWord(QString)));
    connect(this, SIGNAL(chooseError(QString)), player1, SLOT(badChooseLetter(QString)));
    connect(this, SIGNAL(letterChosen()), player1, SLOT(letterChosen()));
    connect(this, SIGNAL(sendBoardFirst(QVector<QVector<QChar> >)), player1, SLOT(setCurrentBoard(QVector<QVector<QChar> >)));

    connect(this, SIGNAL(moveEndedSecond(QString)), player2, SLOT(approveWord(QString)));
    connect(this, SIGNAL(chooseErrorSecond(QString)), player2, SLOT(badChooseLetter(QString)));
    connect(this, SIGNAL(letterChosenSecond()), player2, SLOT(letterChosen()));
    connect(this, SIGNAL(sendBoardSecond(QVector<QVector<QChar> >)), player2, SLOT(setCurrentBoard(QVector<QVector<QChar> >)));

}

void Board::connectToGameManager(QObject* gameManager) {
    connect(this, SIGNAL(sendCellsNumber(int)), gameManager, SLOT(getNumberOfCells(int)));
}


//Methods

void Board::setFirstWord(QString firstWord) {
    for (int i = 0; i <  5; ++i) {
        board_[2][i]->setLetter(firstWord[i]);
    }
}

void Board::setFirstPlayer(int player) {
    currentPlayer = player;
}
void Board::sendError(QString message) {
    if (currentPlayer == FIRST_PLAYER) {
        emit chooseError(message);
    } else {
        emit chooseErrorSecond(message);
    }
}

bool Board::rangeCheck(int x, int y) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        sendError(tr("Wrong range of cell"));
        return false;
    }
    return true;
}

void Board::showBoard(){
    QTextStream out(stdout);
    for(int i = 0; i < HEIGHT; ++i){
        for(int j = 0; j < WIDTH; ++j) {
            out << board_[i][j]->getLetter()<<" ";
        }
        out <<"\n";
    }
}

void Board::changeLetter(int x, int y, QChar letter) {
    if (hasChanged()) {
        sendError(tr("Cell has already choosen"));
        return;
    }
    if (!rangeCheck(x, y)) {
        return;
    }
    if (getLetter(x, y) != QChar('-')) {
        sendError(tr("Error cell chosen"));
        return;
    }

    board_[x][y]->setLetter(letter);
    isChanged = true;
    setMarked(x, y, true);
    setChanged(true);
    showBoard();
    emit addNewLetter(QPair<int,int>(x, y));
    if (currentPlayer == FIRST_PLAYER) {
        emit letterChosen();
    } else {
        emit letterChosenSecond();
    }

}

void Board::pushLetter(QPair<int, int>& coordinates) {
    if (!rangeCheck(coordinates.first, coordinates.second)) {
        return;
    }
    if (getLetter(coordinates.first, coordinates.second) == QChar('-')) {
        return;
    }
    emit commitLetter(getLetter(coordinates.first, coordinates.second));
    emit commitX(coordinates.first);
    emit commitY(coordinates.second);
    if (isMarked(coordinates.first, coordinates.second)) {
        emit commitNew(1);
    } else {
        emit commitNew(0);
    }
}

bool Board::isMarked(int x, int y) {
    return board_[x][y]->isMarked();
}

void Board::setMarked(int x, int y, bool value) {
    board_[x][y]->setMarked(value);
}

QChar Board::getLetter(int x, int y) {
    return board_[x][y]->getLetter();
}

void Board::setLetter(int x, int y, QChar letter) {
    board_[x][y]->setLetter(letter);
}

bool Board::hasChanged() {
    return isChanged;
}

void Board::setChanged(bool changed) {
    isChanged = changed;
}


//slots

void Board::resetState(const QPair<int,int>& coordinates) {
    std::cout<<"reset state from" << coordinates.first << " " << coordinates.second << std::endl;
    std::cout.flush();
    setLetter(coordinates.first, coordinates.second, '-');
    setMarked(coordinates.first, coordinates.second, false);
}

void Board::remakeMove(const QString& word) {

    std::cout<<"New move\n";
    std::cout << std::endl;
    isChanged = false;
    for (int i = 0; i < WIDTH; ++i) {
        for (int j = 0; j < HEIGHT; ++j) {
            if (getLetter(i,j) != '-') {
                setMarked(i, j, false);
            }
        }
    }
    if (isApproved) {
        //switchPlayer;
        isApproved = false;
        currentPlayer = 3 - currentPlayer;
        switch (currentPlayer) {
        case 1:
            emit moveEnded(word);
            break;

        case 2:
            emit moveEndedSecond(word);
            break;
        }
    } else {
        showBoard();
    }
}

void Board::setApproved() {
    isApproved = true;
}

/*
 * Slots from Player
 * */

void Board::chooseLetterFirst(QPair<QPair<int,int>, QChar>& letter) {
    changeLetter(letter.first.first, letter.first.second, letter.second);
    Logger l;
    l.printLog<QString>(DEBUG, tr("GET"));
    l.printLog<int>(DEBUG, letter.first.first);
    l.printLog<int>(DEBUG, letter.first.second);
    l.printLog<QChar>(DEBUG, letter.second);
}

void Board::chooseLetterSecond(QPair<QPair<int,int>,QChar>& letter) {
    changeLetter(letter.first.first, letter.first.second, letter.second);
}

void Board::pushLetterFirst(QPair<int, int>& coordinates) {
    pushLetter(coordinates);
}
void Board::pushLetterSecond(QPair<int, int>& coordinates) {
    pushLetter(coordinates);
}

void Board::showBoardToPlayer() {
    QVector<QVector<QChar> > result(WIDTH, QVector<QChar>(HEIGHT));
    for (int i = 0; i < WIDTH; ++i) {
        for (int j = 0; j < HEIGHT; ++j) {
            result[i][j] = getLetter(i,j);
        }
    }
    if (currentPlayer == FIRST_PLAYER) {
        emit sendBoardFirst(result);
    } else {
        emit sendBoardSecond(result);
    }

}


void Board::gotCommitQuery() {
   emit commitWord();
}

void Board::getNumberOfCells() {
    int cnt = 0;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (getLetter(i, j) == QChar('-')) {
                ++cnt;
            }
        }
    }
    emit sendCellsNumber(cnt);
}

void Board::showBoardToManager() {
    showBoard();
}



