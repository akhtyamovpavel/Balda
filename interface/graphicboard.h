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

#include "buttoncell.h"
#include "gamemanager.h"

class Player;

class GraphicBoard : public QWidget
{
    Q_OBJECT
    QVector<QVector<ButtonCell*> > buttons;
    QGridLayout* tableLayout;
    GameManager* gameManager;
    QVBoxLayout* gamePanel;
    QHBoxLayout* wordPanel;
    QPushButton* commitButton;
    QLabel* currentWord;
public:
    explicit GraphicBoard(QWidget *parent = 0);

    void connectToPlayers(Player* player1, Player* player2);


signals:

    void pushLetter(QPair<int,int> coords);
    void chooseLetter(QPair<QPair<int,int>, QChar> letter);
public slots:
    void onCellPushed();
    void onCellChosen(QChar letter);
};

#endif // GRAPHICBOARD_H
