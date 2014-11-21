#include "buttoncell.h"

ButtonCell::ButtonCell(QToolButton *parent) :
    QToolButton(parent)
{
    connect(this, &ButtonCell::clicked, this, &ButtonCell::onLetterClicked);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    currentMenu_ = createMenu();
    setMenu(currentMenu_);
}

void ButtonCell::connectToPanel(QObject *gb)
{
    connect(this, SIGNAL(onLetterChosen(QChar)), gb, SLOT(onCellChosen(QChar)));
    connect(this, SIGNAL(onLetterPushed()), gb, SLOT(onCellPushed()));
}

QMenu* ButtonCell::createMenu() {
    signalMapper_ = new QSignalMapper(this);
    QMenu* menu = new QMenu(tr("Set of letters"));
    for(wchar_t c = L'а'; c <= L'я'; ++c){
        actions_.push_back(new QAction(QString(c), this));
        menu->addAction(actions_.back());
        signalMapper_->setMapping(actions_.back(), QString(c));
        connect(actions_.back(), SIGNAL(triggered()), signalMapper_, SLOT(map()));
    }
    connect(signalMapper_, SIGNAL(mapped(QString)), this, SLOT(setLetter(QString)));
    return menu;
}
