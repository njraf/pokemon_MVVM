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
    QSharedPointer<AttackMoveDao> attackMoveDao = QSharedPointer<AttackMoveDao>::create();
    QSharedPointer<HealItemDao> healItemDao = QSharedPointer<HealItemDao>::create();
    repository = QSharedPointer<Repository>::create(pokemonDao, attackMoveDao, healItemDao);
    if (!repository->hasConnection()) {
        //TODO: make an error dialog
        qDebug() << "ERROR: Could not connect to repository";
        return;
    }

    // create player
    QVector<QSharedPointer<AttackMove>> attackList;
    QVector<QSharedPointer<AttackMove>> attackList2;
    attackList.append(repository->getAttackByID(1));
    attackList2.append(repository->getAttackByID(2));

    QSharedPointer<Pokemon> charmander = repository->getPokemon(6);
    QSharedPointer<Pokemon> squirtle = repository->getPokemon(7);
    charmander->setAttackList(attackList);
    squirtle->setAttackList(attackList2);
    QVector<QSharedPointer<Pokemon>> playerTeam = {charmander, squirtle};

    QSharedPointer<Bag> bag = QSharedPointer<Bag>::create();
    auto potion = repository->getHealItemByID(1);
    potion->setQuantity(5);
    auto superPotion = repository->getHealItemByID(2);
    superPotion->setQuantity(3);
    auto hyperPotion = repository->getHealItemByID(3);
    hyperPotion->setQuantity(2);
    bag->addHealItem(potion);
    bag->addHealItem(superPotion);
    bag->addHealItem(hyperPotion);
    player = QSharedPointer<Trainer>::create(playerTeam, bag);

    // change pages
    connect(PageNavigator::getInstance(), &PageNavigator::pageChanged, this, [=](QSharedPointer<IPage> page) {
        ui->pages->addWidget(page.data());
        ui->pages->setCurrentIndex(ui->pages->count() - 1);
    });

    // initialize PageNavigator
    QMap<PageName, std::function<QSharedPointer<IPage>(void)> > routes;
    routes.insert(PageName::MAIN_MENU, [=] { return constructMainMenuPage(); });
    routes.insert(PageName::BATTLE, [=] { return constructBattlePage(); });
    routes.insert(PageName::TEAM, [=] { return constructTeamPage(); });
    routes.insert(PageName::POKEMON_SUMMARY, [=] { return constructSummaryPage(repository->getPokemon(9)); });
    routes.insert(PageName::BAG, [=] { return constructBagPage(); });

    PageNavigator::getInstance()->populateRoutes(routes);
    PageNavigator::getInstance()->navigate(PageName::MAIN_MENU);
}

MainWindow::~MainWindow()
{
    delete PageNavigator::getInstance();
    delete ui;
}

QSharedPointer<MainMenuPage> MainWindow::constructMainMenuPage() {
    return QSharedPointer<MainMenuPage>::create();
}

QSharedPointer<BattlePage> MainWindow::constructBattlePage() {

    // currently a static opponent. can change to be dynamic.
    QVector<QSharedPointer<AttackMove>> attackList;
    attackList.append(repository->getAttackByID(2));

    // opponent team
    QSharedPointer<Pokemon> bulbasaur = repository->getPokemon(1);
    QSharedPointer<Pokemon> bulbasaur2 = repository->getPokemon(2);
    QSharedPointer<Pokemon> bulbasaur3 = repository->getPokemon(3);
    bulbasaur->setAttackList(attackList);
    QVector<QSharedPointer<Pokemon>> opponentTeam = {bulbasaur, bulbasaur2, bulbasaur3};
    QSharedPointer<Trainer> opponent = QSharedPointer<Trainer>::create(opponentTeam, QSharedPointer<Bag>::create());
    QSharedPointer<BattleViewmodel> battleViewmodel = QSharedPointer<BattleViewmodel>::create(repository, player, opponent);

    QSharedPointer<BattlePage> battlePage = QSharedPointer<BattlePage>::create(battleViewmodel);
    return battlePage;
}

QSharedPointer<TeamPage> MainWindow::constructTeamPage() {
    QSharedPointer<TeamPage> teamPage = QSharedPointer<TeamPage>::create(player->getTeam());
    return teamPage;
}

QSharedPointer<SummaryPage> MainWindow::constructSummaryPage(QSharedPointer<Pokemon> pokemon) {
    return QSharedPointer<SummaryPage>::create(pokemon);
}

QSharedPointer<BagPage> MainWindow::constructBagPage() {
    QSharedPointer<BagViewmodel> bagViewmodel = QSharedPointer<BagViewmodel>::create(player->getBag());
    QSharedPointer<BagPage> bagPage = QSharedPointer<BagPage>::create(bagViewmodel);
    return bagPage;
}
