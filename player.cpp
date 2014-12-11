#include "player.h"
#include <iostream>
#include <QTextStream>


Player::Player(QObject *parent) :
    QObject(parent)
{
    isChosen = false;
    isCommited = false;
    score = 0;
}


//Connections
void Player::connectToBoard(QObject* board) {
    connect(this, SIGNAL(chooseLetter(QPair<QPair<int,int>,QChar>&)), board,
            SLOT(chooseLetterFirst(QPair<QPair<int,int>,QChar>&)));
    connect(this, SIGNAL(pushLetter(QPair<int,int>&)), board, SLOT(pushLetterFirst(QPair<int,int>&)));
    connect(this, SIGNAL(commitWord()), board, SLOT(gotCommitQuery()));
    connect(this, SIGNAL(showBoard()), board, SLOT(showBoardToPlayer()));
}

void Player::connectToManager(QObject* gameManager) {
    connect(this, SIGNAL(moveEnded()), gameManager, SLOT(stepEnded()));
    connect(this, SIGNAL(dontMakeMove()), gameManager, SLOT(gameEnding()));
}

void Player::connectToInterface(QObject *graphicBoard)
{
    connect(this, SIGNAL(afterLetterChosed(QPair<QPair<int,int>, QChar>)),
                         graphicBoard, SLOT(afterCellChoosen(QPair<QPair<int,int>, QChar>)));
    connect(this, SIGNAL(afterLetterPushed(QPair<int,int>)), graphicBoard, SLOT(afterCellPushed(QPair<int,int>)));
    connect(this, SIGNAL(afterWordCommited(QString)), graphicBoard, SLOT(afterWordCommited(QString)));
    connect(this, SIGNAL(resetWord(const QPair<int,int>&)),
            graphicBoard, SLOT(onPlayerResetWord(const QPair<int,int>&)));
}

void Player::sendEnd()
{
    emit dontMakeMove();
}

//Public methods

int Player::getScore() {
    return score;
}
//Private methods


void Player::runProcess() {
}



//Slots from GameManager
void Player::beginStep() {
    //runProcess();
}


//Slots from Board
void Player::badChooseLetter(QString message) {
    std::cout << message.toStdString() << std::endl;
}

void Player::letterChosen(QPair<QPair<int,int>, QChar> letter) {
    std::cout << "Letter chosen" << std::endl;
    isChosen = true;
    emit afterLetterChosed(letter);
}

void Player::approveWord(QString word) {
    isCommited = true;
    score += word.size();
    listOfWords.push_back(word);
    QTextStream out(stdout);
    out << "Word approved: " << word << "\n";
    out << "Your current score is " << score << "\n";
    out.flush();
    isCommited = false;
    isChosen = false;
    tempCommited = true;
    emit moveEnded();
    emit afterWordCommited(word);

}

void Player::setCurrentBoard(QVector<QVector<QChar> > data) {
    board = data;
}

void Player::onBoardResetWord(const QPair<int, int> &coordinates)
{
    isChosen = false;
    isCommited = false;
    emit resetWord(coordinates);
}

void Player::onLetterChosen(QPair<QPair<int,int>, QChar> letter) {
    if (isChosen) {
        return;
    }
    emit chooseLetter(letter);
}

void Player::onLetterPushed(QPair<int, int> coordinates)
{
    if (!isChosen) {
        return;
    }
    emit pushLetter(coordinates);
    emit afterLetterPushed(coordinates);
}


void Player::onWordCommited()
{
    emit commitWord();
}
