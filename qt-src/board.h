#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QTextStream>
#include <vector>
#include <QPair>
#include <QString>
#include <QVector>
#include "cell.h"

#include "logger.h"

class WordCollector;

class Board : public QObject
{
    Q_OBJECT
    QVector<QVector<Cell*> > board_;
    int currentPlayer;
    bool isChanged;
    bool isApproved;
    const int FIRST_PLAYER = 1;
    const int SECOND_PLAYER = 2;

    int WIDTH;
    int HEIGHT;

    void sendError(QString message);
    bool rangeCheck(int x, int y);

public:

    explicit Board(QObject *parent = 0);

    void setFirstPlayer(int player);
    void setFirstWord(QString firstWord);
    void setUpConnection(WordCollector* wordCollector);

    void connectToPlayers(QObject* player1, QObject* player2);

    void connectToGameManager(QObject* gameManager);

    void showBoard( );

    void changeLetter(int x, int y, QChar letter);

    void pushLetter(QPair<int,int>& coordinates);

    bool isMarked(int x, int y);

    void setMarked(int x, int y, bool value);

    QChar getLetter(int x, int y);

    void setLetter(int x, int y, QChar letter);

    bool hasChanged();

    void setChanged(bool changed);


    void initBoard(int width, int height);
signals:
    /*
     *  Signals to WordCollector
     */
    void commitLetter(QChar letter);
    void commitX(int x);
    void commitY(int y);
    void commitWord();
    void commitNew(int flag);
    void addNewLetter(QPair<int,int> cell);

    /*
     *  Signals to Player
     */

    void moveEnded(QString word);
    void chooseError(QString message);
    void letterChosen(QPair<QPair<int,int>, QChar>);
    void sendBoardFirst(QVector<QVector<QChar> >);
    void resetWordFirst(const QPair<int,int>&);

    void moveEndedSecond(QString word);
    void chooseErrorSecond(QString message);
    void letterChosenSecond(QPair<QPair<int,int>, QChar>);
    void sendBoardSecond(QVector<QVector<QChar> >);
    void resetWordSecond(const QPair<int,int>&);
    /*
     *  Signals to GameManager
     */

    void sendCellsNumber(int value);

public slots:

    /*
     * Slots from WordCollector
     * */
    void resetState(const QPair<int,int>& coordinates);
    void remakeMove(const QString& word);
    void setApproved();


    /*
     * Slots from Player
     * */

    void chooseLetterFirst(QPair<QPair<int,int>,QChar>& letter);
    void chooseLetterSecond(QPair<QPair<int,int>,QChar>& letter);

    void showBoardToPlayer();

    void pushLetterFirst(QPair<int, int>& coordinates);
    void pushLetterSecond(QPair<int, int>& coordinates);

    void gotCommitQuery();

    /*
     *  Slots from GameManager
     */

    void getNumberOfCells();
    void showBoardToManager();



};

#endif // BOARD_H

