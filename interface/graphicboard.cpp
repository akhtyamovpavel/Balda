#include "graphicboard.h"
#include "logger.h"
#include <QInputDialog>
#include "gamemanager.h"

GraphicBoard::GraphicBoard(QWidget *parent) :
    QWidget(parent)
{
    tableLayout = new QGridLayout;
    for (int i = 0; i < 5; ++i) {
        buttons.push_back(QVector<ButtonCell*>(5));
        for (int j = 0; j < 5; ++j) {
            buttons[i][j] = new ButtonCell();
            buttons[i][j]->connectToPanel(this);
            tableLayout->addWidget(buttons[i][j], i, j);
        }
    }
    gamePanel = new QVBoxLayout;
    gamePanel->addLayout(tableLayout);
    wordPanel = new QHBoxLayout;
    currentWord = new QLabel("");
    commitButton = new QPushButton("Enter!");
    wordPanel->addWidget(currentWord);
    wordPanel->addWidget(commitButton);
    gamePanel->addLayout(wordPanel);
    setLayout(gamePanel);
    //WsetLayout(tableLayout);
    int players = QInputDialog::getInt(this, tr("Enter number of players"), tr("Введите число игроков"), 1, 1, 2);
    Logger l;
    l.printLog(DEBUG, players);
    gameManager = new GameManager(players);
    connectToPlayers(gameManager->getFirstPlayer(), gameManager->getSecondPlayer());
    QString word = gameManager->getFirstWord();
    for (int i = 0; i < 5; ++i) {
        buttons[2][i]->setText(QString(word[i]));
    }
    for (int i = 0; i < 5; ++i) {
        buttons[2][i]->setMenu(NULL);
    }
}

void GraphicBoard::connectToPlayers(Player *player1, Player* player2)
{
    connect(this, SIGNAL(chooseLetter(QPair<QPair<int,int>, QChar>)),
            player1, SLOT(onLetterChosen(QPair<QPair<int,int>,QChar>)));
    connect(this, SIGNAL(chooseLetter(QPair<QPair<int,int>, QChar>)),
            player2, SLOT(onLetterChosen(QPair<QPair<int,int>,QChar>)));
    connect(this, SIGNAL(pushLetter(QPair<int,int>)),
            player1, SLOT(onLetterPushed(QPair<int,int>)));
    connect(this, SIGNAL(pushLetter(QPair<int,int>)),
            player2, SLOT(onLetterPushed(QPair<int,int>)));
}


void GraphicBoard::onCellPushed()
{
    int xSender = 0;
    int ySender = 0;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (reinterpret_cast<ButtonCell*>(sender()) == buttons[i][j]) {
                xSender = i;
                ySender = j;
            }
        }
    }
    currentWord->setText(currentWord->text() + buttons[xSender][ySender]->text());
    emit pushLetter(qMakePair(xSender, ySender));
}

void GraphicBoard::onCellChosen(QChar letter)
{
    int xSender = 0;
    int ySender = 0;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (reinterpret_cast<ButtonCell*>(sender()) == buttons[i][j]) {
                xSender = i;
                ySender = j;
            }
        }
    }
    QPair<QPair<int,int>, QChar> sendLetter = qMakePair(qMakePair(xSender, ySender), letter);
    emit chooseLetter(sendLetter);
}




