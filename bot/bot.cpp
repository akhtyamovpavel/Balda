#include "bot.h"
#include <algorithm>
#include <iostream>
#include <QTextStream>
#include "logger.h"
#include <algorithm>
#include <cstdlib>

Bot::Bot(QObject* parent) :
Player(parent)
{
    MOVES.push_back(std::make_pair(0, 1));
    MOVES.push_back(std::make_pair(0, -1));
    MOVES.push_back(std::make_pair(1, 0));
    MOVES.push_back(std::make_pair(-1, 0));
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

    std::cout << "OK" << std::endl;

    std::vector<std::string> notAllowedWords;
    //std::vector<std::vector<char> > table;

    emit showBoard();


    // takeTable(Table);
    // takeListOfNotAllowedWords(notAllowedWords);


    //ofstream out("output.txt");

    QVector<QVector<QChar> > symbols(7, QVector<QChar>(7, QChar('#')));
    for (int i = 1; i <= 5; ++i) {
        for (int j = 1; j <= 5; ++j) {
            symbols[i][j] = board[i-1][j-1];
        }
    }


    std::cout << "OK" << std::endl;
    std::vector<Word> variants = possibleVariants(symbols, std::vector<QString>(0));

    emit showBoard();

    Logger l;

    l.printLog(INFO, variants.size());

    /*for (int i = 0; i < variants.size(); ++ i) {
        l.printLog(DEBUG, variants[i].possibleWord);
        for (int j = 0; j < variants[i].coordinates.size(); ++j) {
            l.printLog(DEBUG, variants[i].coordinates[j].first);
            l.printLog(DEBUG, variants[i].coordinates[j].second);
        }
    }*/

    isCommited = false;

    int usedX = -1;
    int usedY = -1;
    QChar c;
    int id = -1;
    int maxLength = -1;
    /*for (int i = 0; i < variants.size(); ++i) {
        if (variants[i].possibleWord.length() > maxLength) {
            id = i;
            maxLength = variants[i].possibleWord.length();
        }
    }*/

    while(!isCommited) {
        id = 0;
        //id = std::max(rand(), (int)variants.size());
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

        if (isCommited) {
            emit moveEnded();
        }
        ++id;
    }

    //std::cout << "OK" << std::endl;
    //std::cout << variants.size() << std::endl;


    //sendAnswer(possibleVariants);

}

bool Bot::notBelong(std::vector<QString> notAllowedWords, QString &checkIn)
{
    for (int i = 0; i<notAllowedWords.size(); ++i)
        if (notAllowedWords[i] == checkIn)
            return false;

    return true;
}

void Bot::dfs(QVector<QVector<QChar> >  table, std::vector<Word> &words,
              int x, int y, int curPosition, std::vector<std::vector<bool> > curUsed,
              QString &curString,
              std::vector<std::pair<int, int> > &curCoords, bool usedEmpty)
{
    curUsed[x][y] = true;

    if (borVocabulary.borVertices[curPosition].isLeaf() && usedEmpty)  // Если мы пришли в такую позицию, что слово уже готово. Проверяется использованность хоть какой-то пустой клетки.
    {
        curCoords.push_back(std::make_pair(x, y));
        curString += table[x][y];

        words.push_back(Word(curString, curCoords));

        curString = curString.left((int)curString.size() - 1);
        //curString.pop_back();
        curCoords.pop_back();
    }

    if (table[x][y] == QChar('-') && !usedEmpty)  // Если мы пришли в пустую клетку во время работы дфса, то ставим туда всевозможные буквы и пытаемся продлить слово.
    {


        for (int i = 0; i < 32; ++i) {
            //Logger l;
            QChar cc = QChar(1072 + i);
            table[x][y] = cc;
            //l.printLog(DEBUG, cc);
            if (borVocabulary.borVertices[curPosition].findChildren(cc) != -1)
                dfs(table, words, x, y, borVocabulary.borVertices[curPosition].findChildren(cc), curUsed, curString, curCoords, true);
        }
        /*
         * for (int i = 0; i<32; ++i)
        {
            board[x][y] = getChar(i);
            if (borVocabulary.borVertices[curPosition].findChildren(getChar(i)) != -1)
                dfs(words, x, y, borVocabulary.borVertices[curPosition].findChildren(getChar(i)), curUsed, curString, curCoords, true);
        }*/
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

        curCoords.push_back(std::make_pair(x, y));
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

std::vector<Word> Bot::possibleVariants (QVector<QVector<QChar> >& table, std::vector<QString> notAllowedWords)
{
    int N, M;
    N = table.size() - 2;
    M = table[0].size() - 2;
    Logger l;
    l.printLog(DEBUG, table.size());


    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7; ++j) {
            l.printLog(DEBUG, table[i][j]);
            l.printLog(DEBUG, table[i][j].unicode());
        }
    }
    //cerr << N << " " << M << "\n";

    //std::vector<std::string> possibleStrings;
    //std::vector<std::vector<std::pair<int, int> > > coordinates;
    std::vector <Word> words;

    //std::cout << "OK" << std::endl;
    for (int i = 1; i <= N; ++i)
        for (int j = 1; j <= M; ++j)
        {														// Пытаемся запустить дфс из всех клеток таблицы.
        std::vector<std::vector<bool> > curUsed;
        std::vector<std::pair<int, int> > curCoords;
        QString curString;
        curUsed.assign(N + 2, std::vector<bool>(M + 2, false));

        int curPosition = 0;
        //std::cout << "OK" << std::endl;
        if (table[i][j] != QChar('-') && borVocabulary.borVertices[curPosition].findChildren(table[i][j]) != -1)
            dfs(table, words, i, j, borVocabulary.borVertices[curPosition].findChildren(table[i][j]), curUsed, curString, curCoords, false);
        else
            if (table[i][j] == QChar('-'))
                dfs(table, words, i, j, PRE_VERTEX, curUsed, curString, curCoords, false);

        }

    std::vector<Word> corrects;

    for (int i = 0; i < words.size(); ++i)
        if (notBelong(notAllowedWords, words[i].possibleWord))
            corrects.push_back(words[i]);



    return corrects;
}


void Bot::setupDictionary(QVector<QString> words) {
    Logger l;
    //std::cout << words.size() << std::endl;
    l.printLog(DEBUG, tr("Get words"));
    for (int i = 0; i < words.size(); ++i) {
        borVocabulary.addWord(words[i]);
    }
}
