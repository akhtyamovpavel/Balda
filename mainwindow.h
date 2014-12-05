#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "interface/graphicboard.h"
#include <QMenu>
#include <QAction>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


    //actions
    QAction* newGame;
    QAction* exitGame;
    //menus
    QMenu* menu;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    GraphicBoard* gb;


private:
    Ui::MainWindow *ui;
    void createActions();
    void createMenus();

public slots:
    void startNewGame();
};

#endif // MAINWINDOW_H
