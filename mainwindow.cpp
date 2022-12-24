#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pagenavigator.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // create player
    QSharedPointer<Pokemon> charmander = QSharedPointer<Pokemon>::create("Charmander", 10, 5, 9, 4, 10, 10, 10, 5, Type::FIRE);
    QVector<QSharedPointer<Pokemon>> playerTeam = {charmander};
    player = QSharedPointer<Trainer>::create(playerTeam);

    // create startup page
    QSharedPointer<MainMenuPage> menuPage = QSharedPointer<MainMenuPage>::create();
    currentPage = menuPage;
    ui->pages->addWidget(menuPage.data());
    PageNavigator::getInstance()->addToBackstack(menuPage);

    // to new page
    connect(currentPage.data(), &IPage::changedPage, this, [=](PageName pageName) {
        if (pageName == PageName::BATTLE) {
            currentPage = constructBattlePage();
        } else {
            return;
        }

        ui->pages->addWidget(currentPage.data());
        ui->pages->setCurrentIndex(ui->pages->count() - 1);
        PageNavigator::getInstance()->addToBackstack(currentPage);

        // to previous page
        connect(currentPage.data(), &IPage::returnedPage, this, [=] {
            currentPage = PageNavigator::getInstance()->popFromBackstack();
            ui->pages->removeWidget(currentPage.data());
        });
    });

}

MainWindow::~MainWindow()
{
    delete PageNavigator::getInstance();
    delete ui;
}

QSharedPointer<BattlePage> MainWindow::constructBattlePage() {

    // currently a static opponent. can change to be dynamic.
    QSharedPointer<Pokemon> squirtle = QSharedPointer<Pokemon>::create("Squirtle", 7, 9, 10, 4, 10, 7, 10, 5, Type::WATER);
    QVector<QSharedPointer<Pokemon>> opponentTeam = {squirtle};
    QSharedPointer<Trainer> opponent = QSharedPointer<Trainer>::create(opponentTeam);
    QSharedPointer<BattleViewmodel> battleViewmodel = QSharedPointer<BattleViewmodel>::create(player, opponent);

    QSharedPointer<BattlePage> battlePage = QSharedPointer<BattlePage>::create(battleViewmodel);
    return battlePage;
}


