#include "board.h"
#include <iostream>

Board::Board(QObject* wordCollector, QObject *parent) :
    QObject(parent)
{
    isChanged = false;
    board_.resize(5, std::vector<Cell*>(5));
    for (int i = 0; i < 5; ++i){
        for(int j = 0; j <5; ++j) {
            board_[i][j] = new Cell(QChar('-'));
        }
    }

    setUpConnection(wordCollector);
}

void Board::setUpConnection(QObject* wordCollector) {
    connect(this, SIGNAL(commitLetter(int)), wordCollector, SLOT(addLetter(int)));
    connect(this, SIGNAL(commitX(int)), wordCollector, SLOT(addX(int)));
    connect(this, SIGNAL(commitY(int)), wordCollector, SLOT(addY(int)));
    connect(this, SIGNAL(commitWord()), wordCollector, SLOT(checkWord()));
    connect(this, SIGNAL(commitNew(int)), wordCollector, SLOT(addNew(int)));
}

void Board::showBoard(QTextStream& out){
    for(int i = 0; i < 5; ++i){
        for(int j = 0; j < 5; ++j) {
            out << board_[i][j]->getLetter() <<" ";
        }
        out <<"\n";
    }
    out.flush();
}

void Board::changeLetter(int x, int y, QChar letter) {
    board_[x][y]->setLetter(letter);
    isChanged = true;
    setMarked(x, y, true);
    //emit commitLetter(letter.toLatin1() - 'a');
}

void Board::pushLetter(int x, int y) {
    if (getLetter(x,y) == QChar('-')) {
        return;
    }
    emit commitLetter(getLetter(x,y).toLatin1() - 'a');
    emit commitX(x);
    emit commitY(y);
    if (isMarked(x,y)) {
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

void Board::remakeMove() {

    std::cout<<"New move\n";
    std::cout.flush();
    isChanged = false;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (getLetter(i,j) != '-') {
                setMarked(i, j, false);
            }
        }
    }
}
