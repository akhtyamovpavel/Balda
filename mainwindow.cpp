#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "interface/graphicboard.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    createActions();
    createMenus();
    gb = NULL;
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
}

void MainWindow::createMenus()
{
    menu = new QMenu(tr("Game"), this);
    menu->addAction(newGame);
    menu->addSeparator();
    menu->addAction(exitGame);
    menuBar()->addMenu(menu);
}

void MainWindow::startNewGame()
{
    if (gb) {
        delete gb;
    }

    gb = new GraphicBoard(widthLetters, heightLetters, this);
    setCentralWidget(gb);
}
