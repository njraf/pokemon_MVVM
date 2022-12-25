#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pagenavigator.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // create repository
    QSharedPointer<PokemonDao> pokemonDao = QSharedPointer<PokemonDao>::create();
    QSharedPointer<Repository> repository = QSharedPointer<Repository>::create(pokemonDao);
    if (!repository->hasConnection()) {
        //TODO: make an error dialog
        return;
    }

    // create player
    QVector<QSharedPointer<AttackMove>> attackList = {
        QSharedPointer<AttackMove>::create("Ember", 40, 100, 25, 25, Type::FIRE, Category::SPECIAL)
    };
    //QSharedPointer<Pokemon> charmander = QSharedPointer<Pokemon>::create("Charmander", "", NatureUtilities::randomNature(), 39, 52, 43, 60, 50, 65, 39, 5, attackList, Type::FIRE);
    QSharedPointer<Pokemon> charmander = repository->getPokemon(4);
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
    QVector<QSharedPointer<AttackMove>> attackList = {
        QSharedPointer<AttackMove>::create("Water Gun", 40, 100, 25, 25, Type::WATER, Category::SPECIAL)
    };
    QSharedPointer<Pokemon> squirtle = QSharedPointer<Pokemon>::create("Squirtle", "", NatureUtilities::randomNature(), 44, 48, 65, 50, 64, 43, 44, 5, attackList, Type::WATER);
    QVector<QSharedPointer<Pokemon>> opponentTeam = {squirtle};
    QSharedPointer<Trainer> opponent = QSharedPointer<Trainer>::create(opponentTeam);
    QSharedPointer<BattleViewmodel> battleViewmodel = QSharedPointer<BattleViewmodel>::create(repository, player, opponent);

    QSharedPointer<BattlePage> battlePage = QSharedPointer<BattlePage>::create(battleViewmodel);
    return battlePage;
}


