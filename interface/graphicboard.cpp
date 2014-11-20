#include "graphicboard.h"
#include "logger.h"
#include <QInputDialog>

GraphicBoard::GraphicBoard(QWidget *parent) :
    QWidget(parent)
{
    tableLayout = new QGridLayout;
    for (int i = 0; i < 5; ++i) {
        buttons.push_back(QVector<ButtonCell*>(5));
        for (int j = 0; j < 5; ++j) {
            buttons[i][j] = new ButtonCell();
            tableLayout->addWidget(buttons[i][j], i, j);
        }

    }
    setLayout(tableLayout);
    int players = QInputDialog::getInt(this, tr("Enter number of players"), tr("Введите число игроков"), 1, 1, 2);
    Logger l;
    l.printLog(DEBUG, players);
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
    QPair<QChar, QPair<int, int> > sendLetter = qMakePair(letter, qMakePair(xSender, ySender));
    emit chooseLetter(sendLetter);
}


