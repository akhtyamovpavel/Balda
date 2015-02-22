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
    rules();
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

void MainWindow::rules(){
    label = new QLabel(this);
    label->setText("\tПравила игры!\n\tВ игре принимают участие два человека или человек и компьютер.\n\tПервое слово из n букв, случайным образом выбранное компьютером, вписывается в центр игрового поля размером nхn клеток.\nИгроки делают ходы поочередно.\n\tИгрок с помощью левой клавиши мыши выбирает клетку на поле и одну букву из списка.\n Затем игрок последовательно отмечает буквы нового слова, среди которых обязательно должна быть добавленная буква.\nДопустимые направления передвижения: вниз, вверх, влево, вправо.\nПрыжок через клетку и движение по диагонали не допускаются.\n\tЗа каждое новое слово игрок получает столько очков, сколько букв в слове.\n\tМожно составлять только нарицательные имена существительные в начальной форме (единственное число, именительный падеж).\nТакже допускается использование слов, имеющих только форму множественного числа (ножницы, сани, брюки).\nСлова, имеющие форму единственного числа, во множественном числе не принимаются (например, следует использовать слово зулус, а не зулусы).\nРазрешается использование терминов, разговорных и устаревших слов.\nНе принимаются слова, не входящие в состав выбранного литературного языка, а именно: диалектные, просторечные и жаргонные.\nКроме этого, не принимаются аббревиатуры.\nИгра продолжается до заполнения всего игрового поля или истечения лимита времени у одного из игроков.\n\t Побеждает тот, кто набрал наибольшее количество очков.\nСоперник, прервавший игру досрочно, считается сдавшимся и проигрывает.");
    setCentralWidget(label);

}


void MainWindow::startNewGame()
{
    if (gb) {
        delete gb;
    }

    setCentralWidget(NULL);

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

void MainWindow::resetField()
{

    setCentralWidget(NULL);
    rules();
}
