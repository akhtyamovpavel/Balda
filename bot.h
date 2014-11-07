#ifndef BOT_H
#define BOT_H

#include <Qobject>
#include "player.h"

const int NOT_FOUND = -1;
const int PRE_VERTEX = 0;

class Letter{
public:
    char symbol;
    std::vector<int> children;
    std::vector<char> childrenLetters;
    bool leaf;
    Letter();
    Letter(char symbol, bool leaf);
    char getSymbol();
    void setLeaf(bool isNewLeaf);
    void addChildren(int childrenPosition, char symbol);
    int findChildren(char symbol);

};

class Bor{

public:
    std::vector<Letter> borVertices;
    void addLetter(int position, char symbol);
    void addWord(const std::string& word);

};

struct Word
{
    string possibleWord;
    vector<pair<int, int> > coordinates;
};



class Bot : public Player
{
    Q_OBJECT
public:

    Bor borVocabulary;
    vector<pair<int, int> > MOVES;

    explicit Bot(QObject* parent = 0);

    bool notBelong(vector<string> &not_allowed_words, string &check_in);

    void dfs(vector<vector<char> > table, vector<Word> &Words, int x, int y, int cur_position, vector<vector<bool> > cur_used, string &cur_string, vector<pair<int, int> > &cur_coords, bool used_empty);

    vector<Word> possibleVariants(vector<string> &not_allowed_words, vector<vector<char> > &table);


};

#endif // BOT_H
