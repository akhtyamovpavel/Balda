#include "russianlang.h"

RussianLang::RussianLang()
{
    for(wchar_t c = L'А'; c <= L'Е'; ++c){
        rusLetters.push_back(QChar(c));
    }
    QChar c = L'Ё';
    rusLetters.push_back(QChar(c));
    for(wchar_t c = L'Ж'; c <= L'Я'; ++c){
        rusLetters.push_back(QChar(c));
    }
}
