#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "interface/graphicboard.h"
#include "external/settingsdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    createActions();
    createMenus();
    gb = NULL;
    settingsDialog = new SettingsDialog(this, this);
    resize(600, 400);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createActions()
{
    newGame = new QAction(tr("New Game"), this);
    connect(newGame, SIGNAL(triggered()), this, SLOT(startNewGame()));
    exitGame = new QAction(tr("Exit"), this);
    connect(exitGame, SIGNAL(triggered()), qApp, SLOT(quit()));
    settingsAction = new QAction(tr("Settings"), this);
    connect(settingsAction, SIGNAL(triggered()), this, SLOT(runSettings()));
}

void MainWindow::createMenus()
{
    menu = new QMenu(tr("Game"), this);
    menu->addAction(newGame);
    menu->addSeparator();
    menu->addAction(exitGame);
    menuBar()->addMenu(menu);
    settingsMenu = new QMenu(tr("Settings"), this);
    settingsMenu->addAction(settingsAction);
    menuBar()->addMenu(settingsMenu);
}

void MainWindow::startNewGame()
{
    if (gb) {
        delete gb;
    }

    gb = new GraphicBoard(widthLetters, heightLetters, this);
    setCentralWidget(gb);
}

void MainWindow::setSettings(const QPair<int,int>& coordinates) {
     widthLetters = coordinates.first;
     heightLetters = coordinates.second;
}

int MainWindow::getWidth() {
    return widthLetters;
}

int MainWindow::getHeight() {
    return heightLetters;
}

void MainWindow::runSettings() {
    settingsDialog->exec();
}
