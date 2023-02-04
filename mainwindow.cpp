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
    QSharedPointer<AbilityDao> abilityDao = QSharedPointer<AbilityDao>::create();
    QSharedPointer<HealItemDao> healItemDao = QSharedPointer<HealItemDao>::create();
    QSharedPointer<OverworldDao> overworldDao = QSharedPointer<OverworldDao>::create();
    QSharedPointer<PokeballItemDao> pokeballItemDao = QSharedPointer<PokeballItemDao>::create();
    repository = QSharedPointer<Repository>::create(pokemonDao, attackMoveDao, healItemDao, overworldDao, pokeballItemDao, abilityDao);
    if (!repository->hasConnection()) {
        //TODO: make an error dialog
        qDebug() << "ERROR: Could not connect to repository";
        return;
    }

    // create player //
    // create attack list
    QVector<QSharedPointer<AttackMove>> attackList;
    QVector<QSharedPointer<AttackMove>> attackList2;
    attackList.append(repository->getAttackByID(2));
    attackList.append(repository->getAttackByName("Ember", 5));
    attackList.append(repository->getAttackByName("Spore", 4));
    attackList.append(repository->getAttackByName("Confuse Ray", 3));
    attackList2.append(repository->getAttackByID(2));

    // player team
    QSharedPointer<Pokemon> charmander = repository->getPokemon(6);
    QSharedPointer<Pokemon> squirtle = repository->getPokemon(7);
    squirtle->setLevel(99);
    charmander->setAttackList(attackList);
    charmander->setAbility(repository->getAbilityByID(3));
    squirtle->setAttackList(attackList2);
    QVector<QSharedPointer<Pokemon>> playerTeam = {charmander, squirtle};

    // player bag
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
    auto pokeball = repository->getPokeballItemByID(1);
    pokeball->setQuantity(10);
    bag->addPokeballItem(pokeball);
    player = QSharedPointer<Trainer>::create(QSharedPointer<QVector<QSharedPointer<Pokemon>>>::create(playerTeam), bag);

    // change pages
    connect(PageNavigator::getInstance(), &PageNavigator::pageChanged, this, [=](QSharedPointer<IPage> page, bool navigateBack) {
        //qDebug() << "To page" << page->objectName();
        if (navigateBack) {
            ui->pages->removeWidget(page.data());
        } else {
            ui->pages->addWidget(page.data());
            ui->pages->setCurrentIndex(ui->pages->count() - 1);
        }
    });

    // initialize PageNavigator
    QMap<PageName, std::function<QSharedPointer<IPage>(void)> > routes;
    routes.insert(PageName::MAIN_MENU, [=] { return constructMainMenuPage(); });
    routes.insert(PageName::BATTLE, [=] { return constructBattlePage(); });
    routes.insert(PageName::TEAM, [=] { return constructTeamPage(); });
    routes.insert(PageName::POKEMON_SUMMARY, [=] { return constructSummaryPage(); });
    routes.insert(PageName::BAG, [=] { return constructBagPage(); });
    routes.insert(PageName::OVERWORLD, [=] { return constructOverworldPage(); });

    PageNavigator::getInstance()->populateRoutes(routes);
    PageNavigator::getInstance()->navigate(PageName::OVERWORLD);
    ui->pages->currentWidget()->setFocusPolicy(Qt::StrongFocus);
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
    QSharedPointer<BagPage> bagPage = QSharedPointer<BagPage>::create(bagViewmodel);
    return bagPage;
}

QSharedPointer<OverworldPage> MainWindow::constructOverworldPage() {
    auto viewmodel = QSharedPointer<OverworldViewmodel>::create(repository);
    return QSharedPointer<OverworldPage>::create(viewmodel);
}
