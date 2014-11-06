#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QPair>
#include <vector>
#include <QVector>

class Player : public QObject
{
    Q_OBJECT
    std::vector<QString> listOfWords;
    bool isChosen;
    bool isCommited;
    int score;
    QVector<QVector<QChar> > board;


public:
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
};

#endif // PLAYER_H
