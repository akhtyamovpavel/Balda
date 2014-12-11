#ifndef BOT_H
#define BOT_H

#include <QObject>
#include <QVector>
#include <QPair>
#include <QString>
#include "../player.h"
#include "letter.h"
#include "bor.h"
#include <vector>
#include <utility>

#include "logger.h"
#include <lang/language.h>




class Bot : public Player
{
    Q_OBJECT

public:

    const int EASY = 14;
    const int HARD = 88;
    const int HARDEST = 228;
    const int MEDIUM = 265;

    int WIDTH;
    int HEIGHT;

    int level;
    Language *language;
    Bor borVocabulary;
    QVector<QPair<int, int> > MOVES;
    explicit Bot(Language *language, int width, int height, QObject* parent = 0);

    int maximalLength(QVector<Word> variants);

    int easyIndexWord(QVector<Word> variants);

    int mediumIndexWord(QVector<Word> variants);

    int hardIndexWord(QVector<Word> variants);

    int hardestIndexWord(QVector<Word> variants,
                         QVector<QVector<QChar> > symbols,
                         QVector<QString> notAllowedWords);

    QVector<Word> trulyAllowedWord(QVector<Word> variants,
                                   QVector<QString> notAllowedWords);

    void setLevel(int difficulty);

    void connectToDictionary(QObject* dictionary);

    void runProcess() Q_DECL_OVERRIDE;

    bool notBelong(QVector<QString> not_allowed_words,
                   QString &check_in);

    void dfs(QVector<QVector<QChar> >  table,
             QVector<Word> &Words,
             int x,
             int y,
             int cur_position,
             QVector<QVector<bool> > cur_used,
             QString &cur_string,
             QVector<QPair<int, int> > &cur_coords,
             bool used_empty);



    QVector<Word> possibleVariants(QVector<QVector<QChar> >& table);

signals:
    void getDictionary();
    void sendLetter(QPair<QPair<int,int>, QChar> letter);
    void sendWord(QString word);

public slots:

    void beginStep() Q_DECL_OVERRIDE;
    void setupDictionary(QVector<QString> words);
};

#endif // BOT_H
