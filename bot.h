#ifndef BOT_H
#define BOT_H

#include <Qobject>
#include "player.h"

class Bot : public Player
{
    Q_OBJECT
public:
    explicit Bot(QObject* parent = 0);

public slots:
    void runProcess();
};

#endif // BOT_H
