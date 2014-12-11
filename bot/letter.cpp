#include "letter.h"


Letter::Letter(){
    symbol = QChar(' ');
    leaf = false;
}

Letter::Letter(QChar symbol, bool leaf) : symbol(symbol), leaf(leaf){}

QChar Letter::getSymbol(){
    return symbol;
}

bool Letter::isLeaf(){
    return leaf;
}

void Letter::setLeaf(bool isNewLeaf){
    leaf = isNewLeaf;
}

void Letter::addChildren(int childrenPosition, QChar symbol){
    children.push_back(childrenPosition);
    childrenLetters.push_back(symbol);
}

int Letter::findChildren(QChar symbol){
    for (int index = 0; index < childrenLetters.size(); ++index){
        if (childrenLetters.at(index) == symbol){
            return children.at(index);
        }
    }
    return -1;
}
