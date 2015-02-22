#ifndef RUSSIANLANG_H
#define RUSSIANLANG_H
#include <lang/language.h>
#include <QVector>
#include <QChar>

class RussianLang: public Language
{
    QVector <QChar> rusLetters;
public:
    RussianLang();
    const QVector <QChar> getList () const{
        return rusLetters;
    }
};

#endif // RUSSIANLANG_H
