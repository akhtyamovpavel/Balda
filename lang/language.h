#ifndef LANGUAGE_H
#define LANGUAGE_H
#include <QVector>
#include <QChar>

class Language
{
public:
    Language();
    const virtual QVector <QChar> getList () const=0;
};

#endif // LANGUAGE_H
