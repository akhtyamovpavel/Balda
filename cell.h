#ifndef CELL_H
#define CELL_H

#include <QObject>

class Cell : public QObject
{
    Q_OBJECT
    QChar letter_;
    bool isMarked_;

public:
    explicit Cell(QChar let = QChar(), QObject *parent = 0);
    QChar getLetter();

    void setLetter(QChar c);

    bool isMarked();

    void setMarked(bool value);

signals:

public slots:

};

#endif // CELL_H
