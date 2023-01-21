#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pagenavigator.h"
#include "abilityfactory.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // create repository
    QSharedPointer<PokemonDao> pokemonDao = QSharedPointer<PokemonDao>::create();
    QSharedPointer<AttackMoveDao> attackMoveDao = QSharedPointer<AttackMoveDao>::create();
    repository = QSharedPointer<Repository>::create(pokemonDao, attackMoveDao);
    if (!repository->hasConnection()) {
        //TODO: make an error dialog
        qDebug() << "ERROR: Could not connect to repository";
        return;
    }

    // create player
    QVector<QSharedPointer<AttackMove>> attackList;
    attackList.append(repository->getAttackByID(1));

    QSharedPointer<Pokemon> charmander = repository->getPokemon(6);
    charmander->setAttackList(attackList);
    charmander->setAbility(Ability(BattleStage::FAINT, Ability::Target::OPPONENT, AbilityFactory::getAbility(1)));
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
            if (PageNavigator::getInstance()->top()->getPageName() != currentPage->getPageName()) {
                return;
            }
            auto prevPage = PageNavigator::getInstance()->popFromBackstack();
            ui->pages->removeWidget(prevPage.data());
            currentPage = PageNavigator::getInstance()->top();
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
    QVector<QSharedPointer<AttackMove>> attackList;
    attackList.append(repository->getAttackByID(2));

    QSharedPointer<Pokemon> bulbasaur = repository->getPokemon(3);
    bulbasaur->setAttackList(attackList);
    bulbasaur->setAbility(Ability(BattleStage::FAINT, Ability::Target::OPPONENT, AbilityFactory::getAbility(1)));
    QVector<QSharedPointer<Pokemon>> opponentTeam = {bulbasaur};
    QSharedPointer<Trainer> opponent = QSharedPointer<Trainer>::create(opponentTeam);
    QSharedPointer<BattleViewmodel> battleViewmodel = QSharedPointer<BattleViewmodel>::create(repository, player, opponent);

    QSharedPointer<BattlePage> battlePage = QSharedPointer<BattlePage>::create(battleViewmodel);
    return battlePage;
}


