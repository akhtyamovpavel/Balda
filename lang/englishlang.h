#ifndef ENGLISHLANG_H
#define ENGLISHLANG_H
#include <lang/language.h>
#include <QVector>
#include <QChar>

class EnglishLang: public Language
{
    QVector <QChar> engLetters;
public:
    EnglishLang();
    const QVector <QChar> getList () const{
        return engLetters;
    }
};

#endif // ENGLISHLANG_H
