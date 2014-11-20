#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QPair>
#include <vector>
#include <QVector>

#include "logger.h"


class GraphicBoard;

class Player : public QObject
{
    Q_OBJECT
    std::vector<QString> listOfWords;
    bool isChosen;

    int score;

protected:
    QVector<QVector<QChar> > board;



public:

    bool isCommited;
    explicit Player(QObject *parent = 0);
    void connectToBoard(QObject* board);
    void connectToManager(QObject* gameManager);
    void runProcess();
    int getScore();

signals:
    void chooseLetter(QPair<QPair<int,int>, QChar>& letter);
    void pushLetter(QPair<int, int>& coordinates);
    void commitWord();
    void moveEnded();

    void showBoard();

    void afterLetterChosed(QPair<int,int> coordinates);
    void afterLetterPushed(QPair<int,int> coordinates);
    void afterWordCommited(QString word);

public slots:
    /**
     * slots from GameManager
     */
    void beginStep();

    /**
     * slots from Board
     */
    void badChooseLetter(QString message);
    void letterChosen();
    void approveWord(QString word);
    void setCurrentBoard(QVector<QVector<QChar> > data);

    /**
      * Slots from GraphicBoard
      */

    void onLetterChosen(QPair<QPair<int,int>, QChar > letter);
    void onLetterPushed(QPair<int, int> coordinates);
};

#endif // PLAYER_H
