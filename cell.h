#ifndef CELL_H
#define CELL_H



#include <QToolButton>
#include <QMenu>
#include <QAction>
#include <QSignalMapper>
#include <vector>



class Cell: public QToolButton{
    /*
     * Class Cell
     */
    Q_OBJECT
private:
    QChar letter_;
    bool isChoosen_;
    std::vector<QAction*> actions;
    QSignalMapper* signalMapper;


    QMenu* createMenu(){
        signalMapper = new QSignalMapper(this);
        QMenu* menu = new QMenu(tr("Set of lettesrs"));
        for(char c = 'a'; c <= 'z'; ++c){
            actions.push_back(new QAction(QString(c), this));
            menu->addAction(actions.back());
            signalMapper->setMapping(actions.back(), QString(c));
            connect(actions.back(), SIGNAL(triggered()), signalMapper, SLOT(map()));
        }
        connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(setLetter(QString)));
        return menu;
    }


public:
    Cell(const QString& text, QWidget *parent = 0)
        :QToolButton(parent)
    {
        letter_ = QChar();
        isChoosen_ = false;
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        setMenu(createMenu());
    }



    QChar getLetter(){
        return letter_;
    }
private slots:
    void setLetter(QString letter){

        letter_ = letter.at(0);
        QToolButton::setText(letter_);
    }



};




#endif // CELL_H
