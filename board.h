#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QTextStream>
#include <vector>
#include <QPair>
#include <QString>
#include "cell.h"

class Board : public QObject
{
    Q_OBJECT
    std::vector<std::vector<Cell*> > board_;
    bool isChanged;
    bool isApproved;
    const int FIRST_PLAYER = 1;
    const int SECOND_PLAYER = 2;

public:
    explicit Board(QObject *parent = 0);

    void setUpConnection(QObject* wordCollector);

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



signals:
    /*
     *  Signals to WordCollector
     */
    void commitLetter(int value);
    void commitX(int x);
    void commitY(int y);
    void commitWord();
    void commitNew(int flag);

    /*
     *  Signals to Player
     */

    void moveEnded(QString word);
    void chooseError(QString message);
    void letterChosen();

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


    void pushLetterFirst(QPair<int, int>& coordinates);
    void pushLetterSecond(QPair<int, int>& coordinates);

    void gotCommitQuery();

    /*
     *  Slots from GameManager
     */

    void getNumberOfCells();
};

#endif // BOARD_H

