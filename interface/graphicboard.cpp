#include "graphicboard.h"
#include "logger.h"
#include <QInputDialog>
#include "gamemanager.h"
#include <QStringList>
#include <lang/language.h>
#include <lang/englishlang.h>
#include <lang/russianlang.h>

GraphicBoard::GraphicBoard(int width, int height, QWidget *parent) :
    width(width),
    height(height),
    QWidget(parent)
{
    Language *language;
    QStringList langList;
    langList.push_back("Russian");
    langList.push_back("English");
    QString gameLang = QInputDialog::getItem(this, tr("Choose language of the game"), tr("Выберите язык игры"), langList);
    if (gameLang == "Russian"){
        language = new RussianLang;
    }
    else{
        language = new EnglishLang;
    }
    tableLayout = new QGridLayout;
    for (int i = 0; i < height; ++i) {
        buttons.push_back(QVector<ButtonCell*>(width));
        for (int j = 0; j < width; ++j) {
            buttons[i][j] = new ButtonCell(language);
            buttons[i][j]->connectToPanel(this);
            tableLayout->addWidget(buttons[i][j], i, j);
        }
    }
    gamePanel = new QHBoxLayout(this);
    gameBoardPanel = new QVBoxLayout;
    gameBoardPanel->addLayout(tableLayout);
    wordPanel = new QHBoxLayout;
    currentWord = new QLabel("");
    commitButton = new QPushButton("Enter!");
    commitGiveUp = new QPushButton("Give Up");
    wordPanel->addWidget(commitGiveUp);
    wordPanel->addWidget(currentWord);
    wordPanel->addWidget(commitButton);
    gameBoardPanel->addLayout(wordPanel);


    firstPlayerScore = new QLabel(tr("0"), this);
    firstPlayerWords = new QListWidget(this);
    firstPlayerPanel = new QVBoxLayout(this);
    firstPlayerPanel->addWidget(firstPlayerScore);
    firstPlayerPanel->addWidget(firstPlayerWords);

    secondPlayerScore = new QLabel(tr("0"), this);
    secondPlayerWords = new QListWidget(this);
    secondPlayerPanel = new QVBoxLayout(this);
    secondPlayerPanel->addWidget(secondPlayerScore);
    secondPlayerPanel->addWidget(secondPlayerWords);

    gamePanel->addLayout(firstPlayerPanel);
    gamePanel->addLayout(gameBoardPanel);
    gamePanel->addLayout(secondPlayerPanel);

    setLayout(gamePanel);

    int players = QInputDialog::getInt(this, tr("Enter number of players"), tr("Введите число игроков"), 1, 1, 2);
    if (players == 2) {
        gameManager = new GameManager(language, width, height, players);
    } else {
        QStringList list;
        list.push_back(tr("EASY"));
        list.push_back("MEDIUM");
        list.push_back(tr("HARD"));
        list.push_back("HARDEST");
        QString level = QInputDialog::getItem(this, tr("Choose level"), tr("Выберите сложность"), list);
        gameManager = new GameManager(language, width, height, players, level);
    }
    connectToPlayers(gameManager->getFirstPlayer(), gameManager->getSecondPlayer());
    gameManager->getFirstPlayer()->connectToInterface(this);
    gameManager->getSecondPlayer()->connectToInterface(this);
    QString word = gameManager->getFirstWord();
    for (int i = 0; i < width; ++i) {
        buttons[height/2][i]->setText(QString(word[i]));
    }

    for (int i = 0; i < width; ++i) {
        buttons[height/2][i]->setMenu(NULL);
    }

    connect(commitButton, SIGNAL(clicked()), this, SLOT(onCommitButtonClicked()));
    connect(commitGiveUp, SIGNAL(clicked()), this, SLOT(onCommitGiveUpClicked()));


    connect(gameManager, SIGNAL(gameEnded(const QString&)),
            this, SLOT(finishGame(const QString&)));
    connect(this, SIGNAL(quit()), parent, SLOT(resetField()));
}

void GraphicBoard::connectToPlayers(Player *player1, Player* player2)
{
    connect(this, SIGNAL(chooseLetterFirst(QPair<QPair<int,int>, QChar>)),
            player1, SLOT(onLetterChosen(QPair<QPair<int,int>,QChar>)));
    connect(this, SIGNAL(chooseLetterSecond(QPair<QPair<int,int>, QChar>)),
            player2, SLOT(onLetterChosen(QPair<QPair<int,int>,QChar>)));
    connect(this, SIGNAL(pushLetterFirst(QPair<int,int>)),
            player1, SLOT(onLetterPushed(QPair<int,int>)));
    connect(this, SIGNAL(pushLetterSecond(QPair<int,int>)),
            player2, SLOT(onLetterPushed(QPair<int,int>)));
    connect(this, SIGNAL(commitWordFirst()), player1, SLOT(onWordCommited()));
    connect(this, SIGNAL(commitWordSecond()), player2, SLOT(onWordCommited()));
    connect(this, SIGNAL(giveUpFirst()), player1, SLOT(sendEnd()));
    connect(this, SIGNAL(giveUpSecond()), player2, SLOT(sendEnd()));

}


void GraphicBoard::runStep()
{
    gameManager->runGame();
}


void GraphicBoard::onCellPushed()
{
    int xSender = 0;
    int ySender = 0;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (reinterpret_cast<ButtonCell*>(sender()) == buttons[i][j]) {
                xSender = i;
                ySender = j;
            }
        }
    }
    //  currentWord->setText(currentWord->text() + buttons[xSender][ySender]->text());
    if (gameManager->getCurrentPlayer() == 1) {
        emit pushLetterFirst(qMakePair(xSender, ySender));
    } else {
        emit pushLetterSecond(qMakePair(xSender, ySender));
    }
}

void GraphicBoard::onCellChosen(QChar letter)
{
    int xSender = 0;
    int ySender = 0;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (reinterpret_cast<ButtonCell*>(sender()) == buttons[i][j]) {
                xSender = i;
                ySender = j;
            }
        }
    }
    QPair<QPair<int,int>, QChar> sendLetter = qMakePair(qMakePair(xSender, ySender), letter);
    if (gameManager->getCurrentPlayer() == 1) {
        emit chooseLetterFirst(sendLetter);
    } else {
        emit chooseLetterSecond(sendLetter);
    }
}

void GraphicBoard::afterCellPushed(QPair<int, int>coordinates)
{
    currentWord->setText(currentWord->text() + buttons[coordinates.first][coordinates.second]->text());
}

void GraphicBoard::afterCellChoosen(QPair<QPair<int, int>, QChar> coordinates)
{
    buttons[coordinates.first.first][coordinates.first.second]->setMenu(NULL);
    buttons[coordinates.first.first][coordinates.first.second]->setText(QString(coordinates.second));
}

void GraphicBoard::afterWordCommited(QString word) {
    currentWord->setText(NULL);
    if (sender() == gameManager->getFirstPlayer()) {
        firstPlayerWords->addItem(word);
    } else {
        secondPlayerWords->addItem(word);
    }
    if (sender() == gameManager->getFirstPlayer()) {
        int score = gameManager->getFirstPlayer()->getScore();
        firstPlayerScore->setText(QString::number(score));
    } else {
        int score = gameManager->getSecondPlayer()->getScore();
        secondPlayerScore->setText(QString::number(score));
    }
    runStep();
}

void GraphicBoard::onPlayerResetWord(const QPair<int, int> &coordinates)
{
    buttons[coordinates.first][coordinates.second]->createMenu();
    buttons[coordinates.first][coordinates.second]->setText(QString(""));
    currentWord->setText(QString(""));
}

void GraphicBoard::onCommitButtonClicked()
{
    if (gameManager->getCurrentPlayer() == 1) {
        emit commitWordFirst();
    } else {
        emit commitWordSecond();
    }
}

void GraphicBoard::onCommitGiveUpClicked()
{
    if (gameManager->getCurrentPlayer() == 1) {
        emit giveUpFirst();
    } else {
        emit giveUpSecond();
    }
}

void GraphicBoard::finishGame(const QString &message)
{
    QMessageBox box(this);
    box.setText(message);
    QPushButton* okButton = box.addButton(QMessageBox::Ok);
    box.exec();
    if (box.clickedButton() == okButton) {
        emit quit();
    }
}








