#ifndef BUTTONCELL_H
#define BUTTONCELL_H

#include <QWidget>
#include <QToolButton>
#include <QMenu>
#include <QAction>
#include <QSignalMapper>
#include "cell.h"
#include <QMessageBox>
#include <lang/language.h>

class WordCollector;

class GraphicBoard;

class ButtonCell : public QToolButton
{
    Q_OBJECT


    QVector<QAction*> actions_;
    QSignalMapper* signalMapper_;
    QMenu* currentMenu_;
    GraphicBoard* parent_;
    Language* language;

public:
    ButtonCell(Language* language, QToolButton *parent = 0);
    void connectToPanel(QObject *gb);
    void createMenu();

signals:
    void onLetterChosen(QChar letter);
    void onLetterPushed();

public slots:
    void setLetter(QString letter) {
        emit onLetterChosen(letter.at(0));
    }

    void onLetterClicked() {
        emit onLetterPushed();
    }
};

#endif // BUTTONCELL_H
