#ifndef BUTTONCELL_H
#define BUTTONCELL_H

#include <QWidget>
#include <QToolButton>
#include <QMenu>
#include <QAction>
#include <QSignalMapper>
#include "cell.h"
#include <QMessageBox>

class WordCollector;

class GraphicBoard;

class ButtonCell : public QToolButton
{
    Q_OBJECT


    QVector<QAction*> actions_;
    QSignalMapper* signalMapper_;
    QMenu* currentMenu_;
    GraphicBoard* parent_;

    QMenu* createMenu();


public:
    ButtonCell(QToolButton *parent = 0);


signals:
    void onNewLetterChosen(QChar letter);
    void onLetterPushed();

public slots:
    void setLetter(QString letter) {
        setMenu(NULL);
        QToolButton::setText(letter.at(0));
        emit onNewLetterChosen(letter.at(0));
    }

    void onLetterClicked() {
        emit onLetterPushed();
    }
};

#endif // BUTTONCELL_H
