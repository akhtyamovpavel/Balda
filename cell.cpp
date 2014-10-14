#include "cell.h"

Cell::Cell(QChar let, QObject *parent) :
    QObject(parent)
{
    letter_ = let;
    isMarked_ = false;
}

QChar Cell::getLetter(){
    return letter_;
}
void Cell::setLetter(QChar c){
    letter_ = c;
}

bool Cell::isMarked() {
    return isMarked_;
}

void Cell::setMarked(bool value) {
    isMarked_ = value;
}
