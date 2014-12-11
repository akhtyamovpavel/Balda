#include "bor.h"

Bor::Bor(){

    borVertices.push_back(Letter());

}

void Bor::addWord(const QString& word){
    int indexAtTheEndOfWord = 0;
    for (size_t index = 0; index < word.length(); ++index)
    {
        QChar currentLetter = word[index];
        int childrenVertex = borVertices.at(indexAtTheEndOfWord).findChildren(currentLetter);
        if (childrenVertex == NOT_FOUND){
            addLetter(indexAtTheEndOfWord, currentLetter);
            indexAtTheEndOfWord = (int)borVertices.size() - 1;
        }
        else{
            indexAtTheEndOfWord = childrenVertex;
        }

    }
    borVertices.at(indexAtTheEndOfWord).setLeaf(true);
}

void Bor::addLetter(int position, QChar symbol){
    Letter newLetter(symbol, false);
    borVertices.push_back(newLetter);
    borVertices.at(position).addChildren(borVertices.size() - 1, symbol);
}
