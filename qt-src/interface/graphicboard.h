#ifndef GRAPHICBOARD_H
#define GRAPHICBOARD_H

#include <QWidget>
#include <QVector>
#include <QPair>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>

#include "buttoncell.h"
#include "gamemanager.h"

class Player;

class GraphicBoard : public QWidget
{
    Q_OBJECT
    QVector<QVector<ButtonCell*> > buttons;
    QGridLayout* tableLayout;
    GameManager* gameManager;
    QVBoxLayout* gameBoardPanel;
    QHBoxLayout* wordPanel;
    QHBoxLayout* gamePanel;
    QPushButton* commitButton;
    QPushButton* commitGiveUp;
    QLabel* currentWord;
    QListWidget* firstPlayerWords;
    QListWidget* secondPlayerWords;
    QLabel* firstPlayerScore;
    QLabel* secondPlayerScore;
    QVBoxLayout* firstPlayerPanel;
    QVBoxLayout* secondPlayerPanel;

    int width;
    int height;

public:
    explicit GraphicBoard(int width, int height, QWidget *parent = 0);

    void connectToPlayers(Player* player1, Player* player2);

    void runStep();

signals:
    void pushLetterFirst(QPair<int,int> coords);
    void pushLetterSecond(QPair<int,int> coords);
    void chooseLetterFirst(QPair<QPair<int,int>, QChar> letter);
    void chooseLetterSecond(QPair<QPair<int,int>, QChar> letter);
    void commitWordFirst();
    void commitWordSecond();
    void quit();
    void giveUpFirst();
    void giveUpSecond();

public slots:
    void onCellPushed();
    void onCellChosen(QChar letter);
    void afterCellPushed(QPair<int,int> coordinates);
    void afterCellChoosen(QPair<QPair<int,int>, QChar> coordinates);
    void afterWordCommited(QString word);
    void onPlayerResetWord(const QPair<int,int>& coordinates);

    void onCommitButtonClicked();

    void onCommitGiveUpClicked();

    void finishGame(const QString& message);
};

#endif // GRAPHICBOARD_H
