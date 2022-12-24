#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pagenavigator.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSharedPointer<BattlePage> battlePage = constructBattlePage();
    ui->pages->addWidget(battlePage.data());
    PageNavigator::getInstance()->addToBackstack(battlePage);
}

MainWindow::~MainWindow()
{
    delete PageNavigator::getInstance();
    delete ui;
}

QSharedPointer<BattlePage> MainWindow::constructBattlePage() {
    QSharedPointer<Pokemon> charmander = QSharedPointer<Pokemon>::create("Charmander", 1, 1, 1, 1, 1, 1);
    QSharedPointer<Pokemon> squirtle = QSharedPointer<Pokemon>::create("Squirtle", 1, 1, 1, 1, 1, 1);
    QVector<QSharedPointer<Pokemon>> playerTeam = {charmander};
    QVector<QSharedPointer<Pokemon>> opponentTeam = {squirtle};
    QSharedPointer<Trainer> player = QSharedPointer<Trainer>::create(playerTeam);
    QSharedPointer<Trainer> opponent = QSharedPointer<Trainer>::create(opponentTeam);
    QSharedPointer<BattleViewmodel> battleViewmodel = QSharedPointer<BattleViewmodel>::create(player, opponent);

    QSharedPointer<BattlePage> battlePage = QSharedPointer<BattlePage>::create(battleViewmodel);
    return battlePage;
}


