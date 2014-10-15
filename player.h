#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QPair>
#include <vector>

class Player : public QObject
{
    Q_OBJECT
    std::vector<QString> listOfWords;
    bool isChosen;
    bool isCommited;

public:
    explicit Player(QObject *parent = 0);
    void connectToBoard(QObject* board);
    void connectToManager(QObject* gameManager);
    void runProcess();

signals:
    void chooseLetter(QPair<QPair<int,int>, QChar>& letter);
    void pushLetter(QPair<int, int>& coordinates);
    void commitWord();
    void moveEnded();

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
    void approveWord();
};

#endif // PLAYER_H
