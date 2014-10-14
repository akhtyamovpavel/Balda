#include <QCoreApplication>
#include <iostream>
#include <QTextStream>
#include "board.h"
#include "wordcollector.h"
#include "Dictionary.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTextStream in(stdin), out(stdout);

    Dictionary dict;
    WordCollector wc(&dict);

    Board b(&wc);
    wc.connectToBoard(&b);
    dict.setUpConnection(&wc);

    while (true) {
        QString text;
        in>>text;
        if (text == "exit") {
            return 0;
        }
        if(text == "show") {
            b.showBoard(out);
        } else if (text == "new") {
            int x,y;
            in>>x>>y;
            QChar c;
            in>>c;
            if (b.hasChanged()) {
                out << "Cell has already choosen\n";
                out.flush();
                continue;
            }
            if (b.getLetter(x,y) != QChar('-')) {
                out << "error cell choosen\n";
                out.flush();
                continue;
            }
            b.changeLetter(x,y,c);

            b.showBoard(out);
            b.setChanged(true);
        } else if (text == "push") {
            int x, y;
            in >> x >> y;
            if (!b.hasChanged()) {
                out << "choose letter\n";
                out.flush();
            }
            b.pushLetter(x, y);
        } else if (text == "commit"){
            emit b.commitWord();
        } else {
            out << "wrong command\n";
            out.flush();
        }
    }


    return a.exec();
}
