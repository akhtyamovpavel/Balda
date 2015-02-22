#include "buttoncell.h"

ButtonCell::ButtonCell(Language* language, QToolButton *parent) :
    QToolButton(parent),
    language(language)
{
    connect(this, &ButtonCell::clicked, this, &ButtonCell::onLetterClicked);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    createMenu();
}

void ButtonCell::connectToPanel(QObject *gb)
{
    connect(this, SIGNAL(onLetterChosen(QChar)), gb, SLOT(onCellChosen(QChar)));
    connect(this, SIGNAL(onLetterPushed()), gb, SLOT(onCellPushed()));
}

void ButtonCell::createMenu() {
    signalMapper_ = new QSignalMapper(this);
    QMenu* menu = new QMenu(tr("Set of letters"));
    QVector <QChar> currentLang = language->getList();
    for(int i = 0; i<currentLang.size(); i++){
        actions_.push_back(new QAction(QString(currentLang[i]), this));
        menu->addAction(actions_.back());
        signalMapper_->setMapping(actions_.back(), QString(currentLang[i]));
        connect(actions_.back(), SIGNAL(triggered()), signalMapper_, SLOT(map()));
    }
    connect(signalMapper_, SIGNAL(mapped(QString)), this, SLOT(setLetter(QString)));
    setMenu(menu);
}
