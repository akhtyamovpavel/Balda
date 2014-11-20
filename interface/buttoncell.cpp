#include "buttoncell.h"

ButtonCell::ButtonCell(QToolButton *parent) :
    QToolButton(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    currentMenu_ = createMenu();
    setMenu(currentMenu_);
}

QMenu* ButtonCell::createMenu() {
    signalMapper_ = new QSignalMapper(this);
    QMenu* menu = new QMenu(tr("Set of lettesrs"));
    for(char c = 'a'; c <= 'z'; ++c){
        actions_.push_back(new QAction(QString(c), this));
        menu->addAction(actions_.back());
        signalMapper_->setMapping(actions_.back(), QString(c));
        connect(actions_.back(), SIGNAL(triggered()), signalMapper_, SLOT(map()));
    }
    connect(signalMapper_, SIGNAL(mapped(QString)), this, SLOT(setLetter(QString)));
    return menu;
}
