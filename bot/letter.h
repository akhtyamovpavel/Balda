#ifndef LETTER_H
#define LETTER_H

#include <vector>
#include <QChar>
#include <QVector>

class Letter{
public:
    QChar symbol;
    std::vector<int> children;
    std::vector<QChar> childrenLetters;
    bool leaf;
    Letter();
    Letter(QChar symbol, bool leaf);
    QChar getSymbol();
    void setLeaf(bool isNewLeaf);
    void addChildren(int childrenPosition, QChar symbol);
    int findChildren(QChar symbol);
    bool isLeaf();

};

#endif // LETTER_H
