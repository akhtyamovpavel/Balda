#ifndef GRAPHICBOARD_H
#define GRAPHICBOARD_H

#include <QWidget>
#include <QVector>
#include <QPair>
#include <QGridLayout>
#include "buttoncell.h"
class Player;

class GraphicBoard : public QWidget
{
    Q_OBJECT
    QVector<QVector<ButtonCell*> > buttons;
    QGridLayout* tableLayout;

public:
    explicit GraphicBoard(QWidget *parent = 0);

signals:

    void pushLetter(QPair<int,int> coords);
    void chooseLetter(QPair<QChar, QPair<int, int> > letter);
public slots:
    void onCellPushed();
    void onCellChosen(QChar letter);
};

#endif // GRAPHICBOARD_H
