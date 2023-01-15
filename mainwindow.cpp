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
    QSharedPointer<OverworldDao> overworldDao = QSharedPointer<OverworldDao>::create();
    QSharedPointer<OwnedPokemonDao> ownedPokemonDao = QSharedPointer<OwnedPokemonDao>::create();
    QSharedPointer<OwnedPokemonAttackMoveDao> ownedPokemonAttackMoveDao = QSharedPointer<OwnedPokemonAttackMoveDao>::create();
    repository = QSharedPointer<Repository>::create(pokemonDao, attackMoveDao, healItemDao, overworldDao, ownedPokemonDao, ownedPokemonAttackMoveDao);
    if (!repository->hasConnection()) {
        //TODO: make an error dialog
        qDebug() << "ERROR: Could not connect to repository";
        return;
    }

    // create player
    populateSampleTrainerPokemon();

    QVector<QSharedPointer<Pokemon>> playerTeam = repository->getPartyPokemon();


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
    routes.insert(PageName::POKEMON_SUMMARY, [=] { return constructSummaryPage(); });
    routes.insert(PageName::BAG, [=] { return constructBagPage(); });
    routes.insert(PageName::OVERWORLD, [=] { return constructOverworldPage(); });
    routes.insert(PageName::BOX, [=] { return constructBoxPage(); });

    PageNavigator::getInstance()->populateRoutes(routes);
    PageNavigator::getInstance()->navigate(PageName::OVERWORLD);
    ui->pages->currentWidget()->setFocusPolicy(Qt::StrongFocus);
}

MainWindow::~MainWindow()
{
    delete PageNavigator::getInstance();
    delete ui;
}

void MainWindow::populateSampleTrainerPokemon() {
    QVector<QSharedPointer<AttackMove>> attackList;
    attackList.append(repository->getAttackByID(2));
    attackList.append(repository->getAttackByName("Will-O-Wisp", 5));
    attackList.append(repository->getAttackByName("Spore", 4));
    attackList.append(repository->getAttackByName("Confuse Ray", 3));
    QSharedPointer<Pokemon> charmander = repository->getNewPokemon(6);
    charmander->setBoxNumber(0);
    charmander->setAttackList(attackList);
    repository->addNewPokemon(charmander);

    QVector<QSharedPointer<AttackMove>> attackList2;
    attackList2.append(repository->getAttackByID(2));
    QSharedPointer<Pokemon> squirtle = repository->getNewPokemon(7);
    squirtle->setLevel(99);
    squirtle->setBoxNumber(0);
    squirtle->setAttackList(attackList2);
    repository->addNewPokemon(squirtle);

    QVector<QSharedPointer<AttackMove>> attackList3;
    attackList3.append(repository->getAttackByID(1));
    QSharedPointer<Pokemon> p = repository->getNewPokemon(9);
    p->setLevel(50);
    p->setBoxNumber(1);
    p->setAttackList(attackList3);
    repository->addNewPokemon(p);

    QVector<QSharedPointer<AttackMove>> attackList4;
    attackList4.append(repository->getAttackByID(4));
    QSharedPointer<Pokemon> p2 = repository->getNewPokemon(3);
    p2->setLevel(40);
    p2->setBoxNumber(1);
    p2->setAttackList(attackList4);
    repository->addNewPokemon(p2);
}

QSharedPointer<MainMenuPage> MainWindow::constructMainMenuPage() {
    return QSharedPointer<MainMenuPage>::create();
}

QSharedPointer<BattlePage> MainWindow::constructBattlePage() {
    QSharedPointer<BattleViewmodel> battleViewmodel = QSharedPointer<BattleViewmodel>::create(repository, player);
    QSharedPointer<BattlePage> battlePage = QSharedPointer<BattlePage>::create(battleViewmodel);
    return battlePage;
}

QSharedPointer<TeamPage> MainWindow::constructTeamPage() {
    QSharedPointer<TeamPage> teamPage = QSharedPointer<TeamPage>::create(player->getTeam());
    return teamPage;
}

QSharedPointer<SummaryPage> MainWindow::constructSummaryPage() {
    return QSharedPointer<SummaryPage>::create();
}

QSharedPointer<BagPage> MainWindow::constructBagPage() {
    QSharedPointer<BagViewmodel> bagViewmodel = QSharedPointer<BagViewmodel>::create(player->getBag());
    return QSharedPointer<BagPage>::create(bagViewmodel);
}

QSharedPointer<OverworldPage> MainWindow::constructOverworldPage() {
    auto viewmodel = QSharedPointer<OverworldViewmodel>::create(repository);
    return QSharedPointer<OverworldPage>::create(viewmodel);
}

QSharedPointer<BoxPage> MainWindow::constructBoxPage() {
    auto viewmodel = QSharedPointer<BoxViewmodel>::create(repository, player->getTeam());
    return QSharedPointer<BoxPage>::create(viewmodel);
}
