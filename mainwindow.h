#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "interface/graphicboard.h"
#include <QMenu>
#include <QAction>
#include "external/settingsdialog.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


    SettingsDialog dialog;
    int widthLetters = 5;
    int heightLetters = 5;

    //actions
    QAction* newGame;
    QAction* exitGame;
    QAction* settingsMenu;
    //menus
    QMenu* menu;
    QMenu* settings;

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
