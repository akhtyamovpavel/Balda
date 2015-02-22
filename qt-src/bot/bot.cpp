#include "bot.h"
#include <algorithm>
#include <iostream>
#include <QTextStream>
#include "logger.h"
#include <algorithm>
#include <cstdlib>
#include <string>


Bot::Bot(Language *language, int width, int height, QObject* parent) :
    WIDTH(width),
    HEIGHT(height),
    Player(parent),
    language(language)
{
    MOVES.push_back(qMakePair(0, 1));
    MOVES.push_back(qMakePair(0, -1));
    MOVES.push_back(qMakePair(1, 0));
    MOVES.push_back(qMakePair(-1, 0));
}

int Bot::maximalLength(QVector<Word> variants) {

    int maxx = 0;
    for(int i = 0; i<variants.size(); ++i) {
        if (variants[i].possibleWord.length() > maxx)
            maxx = variants[i].possibleWord.length();
    }

    return maxx;

}

QVector<Word> Bot::trulyAllowedWord(QVector<Word> variants, QVector<QString> notAllowedWords) {
    QVector<Word> trulyAllowedVariants;
    for (int i = 0; i<variants.size(); ++i) {
       bool possible = true;
       for(int j = 0; j<notAllowedWords.size(); ++j) {
           if (variants[i].possibleWord == notAllowedWords[j]) {
               possible = false;
           }
       }
       if (possible) {
           trulyAllowedVariants.push_back(variants[i]);
       }
    }

    return trulyAllowedVariants;

}

int Bot::easyIndexWord(QVector<Word> variants) {

    int sz = variants.size() - 1;
    int index = rand()%sz;
    return index;

}

int Bot::mediumIndexWord(QVector<Word> variants) {

    int sz = variants.size() - 1;

    int val = 0;
    for (int i = 0; i < sz; ++i) {
        val += variants[i].possibleWord.length()*variants[i].possibleWord.length();
    }

    int ran = rand()%val;

    int cur = 0;
    for(int i = 0; i < variants.size(); ++i) {
        cur += variants[i].possibleWord.length()*variants[i].possibleWord.length();
        if (cur >= ran)
            return i;
    }

}

int Bot::hardIndexWord(QVector<Word> variants) {

    int sz = variants.size();
    if (sz == 0)
        return -1;

    int maxx = -1;
    for(int i = 0; i<sz; ++i) {
        Logger l;
        l.printLog(DEBUG, variants[i].possibleWord);
        if (variants[i].possibleWord.length() > maxx) {
            maxx = variants[i].possibleWord.length();
        }
    }

    Logger l;
    l.printLog(DEBUG, "meaning of maxx");
    l.printLog(DEBUG, maxx);

    std::vector<Word> maximalVariants;

    for(int i = 0; i<sz; ++i) {
        if (variants[i].possibleWord.length() == maxx) {
            maximalVariants.push_back(variants[i]);
        }
    }

    int newSz = maximalVariants.size();
    if (newSz == 0)
        return -1;

    int indexNewVector = rand()%newSz;

    for(int index = 0; index < variants.size(); ++index) {
        if (variants[index].possibleWord == maximalVariants[indexNewVector].possibleWord) {
            return index;
        }
    }

}

int Bot::hardestIndexWord(QVector<Word> variants, QVector<QVector<QChar> > symbols, QVector<QString> notAllowedWords) {

    int bestIndex = -1;
    int maxx = -111111;
    for(int index = 0; index < variants.size(); ++index) {
        int usedX;
        int usedY;
        QChar c;
        for (int i = 0; i < variants[index].coordinates.size(); ++i) {
            int x = variants[index].coordinates[i].first;
            int y = variants[index].coordinates[i].second;
            if (symbols[x][y] == QChar('-')) {
                usedX = x;
                usedY = y;
                c = variants[index].possibleWord[i];
            }
        notAllowedWords.push_back(variants[index].possibleWord);
        int del1 = variants[index].possibleWord.length();
        symbols[usedX][usedY] = c;
        QVector<Word> newVariants = possibleVariants(symbols);
        symbols[usedX][usedY] = QChar('-');
        notAllowedWords.back();
        QVector<Word> newGoodVariants = trulyAllowedWord(newVariants, notAllowedWords);
        int del2 = maximalLength(newGoodVariants);
        if (del1 - del2 > maxx) {
            maxx = del1 - del2;
            bestIndex = index;
        }
    }
    }
    return bestIndex;
}


void Bot::connectToDictionary(QObject* dictionary) {
    connect(this, SIGNAL(getDictionary()), dictionary, SLOT(sendDictionary()));
    emit getDictionary();
}



void Bot::beginStep() {
    runProcess();
}
/*
*  This slot realizes the move of Bot and interacts with error signals from other classes
*
*  Strategy
*      @author Aliaksei Semchankau
*  Interface
*      @author Pavel Akhtyamov
*/
void Bot::runProcess() {


    QVector<QString> notAllowedWords;

    emit showBoard();

    QVector<QVector<QChar> > symbols(HEIGHT + 2, QVector<QChar>(WIDTH + 2, QChar('#')));
    for (int i = 1; i <= HEIGHT; ++i) {
        for (int j = 1; j <= WIDTH; ++j) {
            symbols[i][j] = board[i-1][j-1];
        }
    }



    QVector<Word> variants = possibleVariants(symbols);

    emit showBoard();

    Logger l;



    isCommited = false;  

    int id = -1;

    if (level == EASY) {
        id = easyIndexWord(variants);
    }

    if (level == MEDIUM) {
        id = mediumIndexWord(variants);
    }

    if (level == HARD) {
        id = hardIndexWord(variants);
    }

    if (level == HARDEST) {
        id = hardestIndexWord(variants, symbols, notAllowedWords);
    }


     if (id == -1) {
         emit dontMakeMove();
     }

    int usedX;
    int usedY;
    QChar c;
    while(!tempCommited) {

        l.printLog(DEBUG, variants[id].possibleWord);
        for (int i = 0; i < variants[id].coordinates.size(); ++i) {
            int x = variants[id].coordinates[i].first - 1;
            int y = variants[id].coordinates[i].second - 1;
            if (board[x][y] == QChar('-')) {
                usedX = x;
                usedY = y;
                c = variants[id].possibleWord[i];
            }
        }

        l.printLog(DEBUG, usedX);
        l.printLog(DEBUG, usedY);
        l.printLog(DEBUG, c);
        QPair<QPair<int,int>, QChar> res(QPair<int,int>(usedX,usedY), c);
        emit chooseLetter(res);

        for (int i = 0; i < variants[id].coordinates.size(); ++i) {
            int x = variants[id].coordinates[i].first - 1;
            int y = variants[id].coordinates[i].second - 1;
            QPair<int,int> coords(x, y);
            l.printLog(DEBUG, "push");
            l.printLog(DEBUG, x);
                l.printLog(DEBUG, y);
            emit pushLetter(coords);
        }

        emit commitWord();

        if (tempCommited) {
            tempCommited = false;
            return;
        }
        //++id;
        QVector<Word> :: iterator it = variants.begin() + id;
        variants.erase(it);

        if (level == EASY) {
            id = easyIndexWord(variants);
        }

        if (level == HARD) {
            id = hardIndexWord(variants);
        }

        if (level == MEDIUM) {
            id = mediumIndexWord(variants);
        }


        if (level == HARDEST) {
            id = hardestIndexWord(variants, symbols, notAllowedWords);
        }


         if (id == -1) {
             emit dontMakeMove();
         }


    }


}

void Bot::setLevel(int difficulty) {
    level = difficulty;
}

bool Bot::notBelong(QVector<QString> notAllowedWords, QString &checkIn) {
    for (int i = 0; i<notAllowedWords.size(); ++i)
        if (notAllowedWords[i] == checkIn)
            return false;

    return true;
}

void Bot::dfs(QVector<QVector<QChar> >  table, QVector<Word> &words,
              int x, int y, int curPosition, QVector<QVector<bool> > curUsed,
              QString &curString,
              QVector<QPair<int, int> > &curCoords, bool usedEmpty)
{
    curUsed[x][y] = true;

    if (borVocabulary.borVertices[curPosition].isLeaf() && usedEmpty)  // Если мы пришли в такую позицию, что слово уже готово. Проверяется использованность хоть какой-то пустой клетки.
    {
        curCoords.push_back(qMakePair(x, y));
        curString += table[x][y];

        words.push_back(Word(curString, curCoords));

        curString = curString.left((int)curString.size() - 1);
        curCoords.pop_back();
    }

    if (table[x][y] == QChar('-') && !usedEmpty)  // Если мы пришли в пустую клетку во время работы дфса, то ставим туда всевозможные буквы и пытаемся продлить слово.
    {

    QVector <QChar> letters = language->getList();
        for (int i=0; i < letters.size(); i++) {
            QChar cc = QChar(letters[i]);
            table[x][y] = cc;
            if (borVocabulary.borVertices[curPosition].findChildren(cc) != -1)
                dfs(table, words, x, y, borVocabulary.borVertices[curPosition].findChildren(cc), curUsed, curString, curCoords, true);
        }
        table[x][y] = QChar('-');
    }
    else
        for (size_t i = 0; i<MOVES.size(); ++i)  // Иначе мы пытаемся пройти в одном из четырех направлений и продлить слово.
        {
        int dx = MOVES[i].first;
        int dy = MOVES[i].second;

        int xx = x + dx;
        int yy = y + dy;

        if (table[xx][yy] == QChar('#'))
            continue;

        if (table[xx][yy] == QChar('-') && usedEmpty)
            continue;

        if (curUsed[xx][yy])
            continue;

        curCoords.push_back(qMakePair(x, y));
        curString += table[x][y];

        if (table[xx][yy] != QChar('-'))
        {
            if (borVocabulary.borVertices[curPosition].findChildren(table[xx][yy]) != NOT_FOUND)
                dfs(table, words, xx, yy, borVocabulary.borVertices[curPosition].findChildren(table[xx][yy]), curUsed, curString, curCoords, usedEmpty);
        }
        else
        {
            dfs(table, words, xx, yy, curPosition, curUsed, curString, curCoords, usedEmpty);
        }

        curString = curString.left((int)curString.size() - 1);
        //curString.pop_back();
        curCoords.pop_back();
    }

    curUsed[x][y] = false;
}

QVector<Word> Bot::possibleVariants (QVector<QVector<QChar> >& table)
{
    int N, M;
    N = table.size() - 2;
    M = table[0].size() - 2;
    QVector<Word> words;

    for (int i = 1; i <= N; ++i)
        for (int j = 1; j <= M; ++j)
        {														// Пытаемся запустить дфс из всех клеток таблицы.
        QVector<QVector<bool> > curUsed(N + 2, QVector<bool>(M + 2, false));
        QVector<QPair<int, int> > curCoords;
        QString curString;

        int curPosition = 0;
        if (table[i][j] != QChar('-') && borVocabulary.borVertices[curPosition].findChildren(table[i][j]) != -1)
            dfs(table, words, i, j, borVocabulary.borVertices[curPosition].findChildren(table[i][j]), curUsed, curString, curCoords, false);
        else
            if (table[i][j] == QChar('-'))
                dfs(table, words, i, j, PRE_VERTEX, curUsed, curString, curCoords, false);

        }

    QVector<Word> corrects;

    for (int i = 0; i < words.size(); ++i)
        corrects.push_back(words[i]);

    return corrects;
}


void Bot::setupDictionary(QVector<QString> words) {
    Logger l;
    l.printLog(DEBUG, tr("Get words"));
    for (int i = 0; i < words.size(); ++i) {
        borVocabulary.addWord(words[i]);
    }
}


