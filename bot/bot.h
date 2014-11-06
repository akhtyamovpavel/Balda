#ifndef BOT_H
#define BOT_H

#include <Qobject>
#include <QVector>
#include <QString>
#include "../player.h"
#include "letter.h"
#include "bor.h"
#include <vector>
#include <utility>





class Bot : public Player
{
    Q_OBJECT

public:

    Bor borVocabulary;
    std::vector<std::pair<int, int> > MOVES;
    explicit Bot(QObject* parent = 0);

    void connectToDictionary(QObject* dictionary);

    bool notBelong(std::vector<QString> not_allowed_words, QString &check_in);

    void dfs(std::vector<Word> &Words, int x, int y, int cur_position,
             std::vector<std::vector<bool> > cur_used,
             QString &cur_string,
             std::vector<std::pair<int, int> > &cur_coords,
             bool used_empty);

    int getCode(char c){
        return (c - 'а');
    }

    char getChar(int code){
        return char('а' + code);
    }

    std::vector<Word> possibleVariants(std::vector<QString> not_allowed_words);

signals:
    void getDictionary();

public slots:
    void runProcess();

    void setupDictionary(QVector<QString> words);
};

#endif // BOT_H
