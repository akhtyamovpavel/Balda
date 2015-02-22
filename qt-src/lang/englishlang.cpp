#include "englishlang.h"

EnglishLang::EnglishLang()
{
    for(wchar_t c = L'A'; c <= L'Z'; ++c){
        engLetters.push_back(QChar(c));
    }
}
