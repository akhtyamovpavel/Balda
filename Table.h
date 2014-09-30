#ifndef TABLE_H
#define TABLE_H

#include "Cell.h"

#include <vector>

#include <QGridLayout>
#include <QWidget>

class Table: public QWidget {
    Q_OBJECT
    std::vector<std::vector<Cell*> > buttons;
    QGridLayout *tableLayout;



public:

    Table(int rows, int columns, QWidget *parent = 0)
        :QWidget(parent)
    {
        tableLayout = new QGridLayout;
        buttons.assign(rows, std::vector<Cell*>(columns));
        for(int i = 0; i < rows; ++i){
            for(int j = 0; j < columns; ++j){
                buttons[i][j] = new Cell(tr("ff"));
                tableLayout->addWidget(buttons[i][j], i, j);
            }
        }
        this->setLayout(tableLayout);
    }

    QChar getLetterFromTable(int row, int column){
        return buttons[row][column]->getLetter();
    }

};




#endif // TABLE_H
