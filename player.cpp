#include "player.h"
#include <iostream>
#include <QTextStream>


Player::Player(QObject *parent) :
    QObject(parent)
{
    isChosen = false;
    isCommited = false;
}


//Connections
void Player::connectToBoard(QObject* board) {
    connect(this, SIGNAL(chooseLetter(QPair<QPair<int,int>,QChar>&)), board,
            SLOT(chooseLetterFirst(QPair<QPair<int,int>,QChar>&)));
    connect(this, SIGNAL(pushLetter(QPair<int,int>&)), board, SLOT(pushLetterFirst(QPair<int,int>&)));
    connect(this, SIGNAL(commitWord()), board, SLOT(gotCommitQuery()));
}

void Player::connectToManager(QObject* gameManager) {
    connect(this, SIGNAL(moveEnded()), gameManager, SLOT(stepEnded()));
}

//Private methods


void Player::runProcess() {
    QTextStream in(stdin);
    QTextStream out(stdout);
    while (!isCommited) {
        QString text;
        in>>text;
        if (text == "exit") {
            //do some;
        }
        if(text == "show") {

        } else if (text == "new") {
            int x,y;
            in>>x>>y;
            QChar c;
            in>>c;
            QPair<QPair<int, int>, QChar> sendLetter(QPair<int,int>(x,y), c);
            emit chooseLetter(sendLetter);

        } else if (text == "push") {
            int x, y;
            in >> x >> y;
            if (!isChosen) {
                out << "Choose letter first\n";
                out.flush();
            }
            QPair<int,int> coordinates(x,y);
            emit pushLetter(coordinates);
        } else if (text == "commit"){
            emit commitWord();
        } else {
            out << "wrong command\n";
            out.flush();
        }
    }
    isChosen = false;
    isCommited = false;
    emit moveEnded();
}



//Slots from GameManager
void Player::beginStep() {
    runProcess();
}


//Slots from Board
void Player::badChooseLetter(QString message) {
    std::cout << message.toStdString() << std::endl;
}

void Player::letterChosen() {
    isChosen = true;
}

void Player::approveWord() {
    isCommited = true;
    //send to GameManager;
}


