#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QApplication>
#include <QMainWindow>
#include "interface/graphicboard.h"
#include <QMenu>
#include <QAction>
#include <QLabel>
#include <QString>

class SettingsDialog;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


    SettingsDialog* settingsDialog;
    int widthLetters = 5;
    int heightLetters = 5;

    //actions
    QAction* newGame;
    QAction* exitGame;
    QAction* settingsAction;
    //menus
    QMenu* menu;
    QMenu* settingsMenu;
    //rules
    QLabel* label;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    GraphicBoard* gb;

    int getWidth();
    int getHeight();
private:
    Ui::MainWindow *ui;
    void createActions();
    void createMenus();
    void rules();


public slots:
    void startNewGame();
    void setSettings(const QPair<int, int> &coordinates);
    void runSettings();
    void resetField();
};

#endif // MAINWINDOW_H
