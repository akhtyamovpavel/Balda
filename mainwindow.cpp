#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    Table* table = new Table(5,5);

    this->setCentralWidget(table);
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

