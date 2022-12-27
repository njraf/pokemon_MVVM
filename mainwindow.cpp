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
    repository = QSharedPointer<Repository>::create(pokemonDao, attackMoveDao);
    if (!repository->hasConnection()) {
        //TODO: make an error dialog
        qDebug() << "ERROR: Could not connect to repository";
        return;
    }

    // create player
    QVector<QSharedPointer<AttackMove>> attackList;
    attackList.append(repository->getAttackByID(3));

    QSharedPointer<Pokemon> charmander = repository->getPokemon(6);
    charmander->setAttackList(attackList);
    QVector<QSharedPointer<Pokemon>> playerTeam = {charmander};
    player = QSharedPointer<Trainer>::create(playerTeam);

    // change pages
    connect(PageNavigator::getInstance(), &PageNavigator::pageChanged, this, [=](QSharedPointer<IPage> page) {
        int asd = ui->pages->addWidget(page.data());
        ui->pages->setCurrentIndex(ui->pages->count() - 1);
    });

    // initialize PageNavigator
    QMap<PageName, std::function<QSharedPointer<IPage>(QVector<QVariant>)> > routes;
    routes.insert(PageName::MAIN_MENU, [=](QVector<QVariant> data) { return constructMainMenuPage(); });
    routes.insert(PageName::BATTLE, [=](QVector<QVariant> data) { return constructBattlePage(); });
    routes.insert(PageName::TEAM, [=](QVector<QVariant> data) { return constructTeamPage(); });
    routes.insert(PageName::POKEMON_SUMMARY, [=](QVector<QVariant> data) { return constructSummaryPage(data); });

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

    QSharedPointer<Pokemon> bulbasaur = repository->getPokemon(3);
    bulbasaur->setAttackList(attackList);
    QVector<QSharedPointer<Pokemon>> opponentTeam = {bulbasaur};
    QSharedPointer<Trainer> opponent = QSharedPointer<Trainer>::create(opponentTeam);
    QSharedPointer<BattleViewmodel> battleViewmodel = QSharedPointer<BattleViewmodel>::create(repository, player, opponent);

    QSharedPointer<BattlePage> battlePage = QSharedPointer<BattlePage>::create(battleViewmodel);
    return battlePage;
}

QSharedPointer<TeamPage> MainWindow::constructTeamPage() {
    QSharedPointer<TeamPage> teamPage = QSharedPointer<TeamPage>::create(player->getTeam());
    return teamPage;
}

QSharedPointer<SummaryPage> MainWindow::constructSummaryPage(QVector<QVariant> data) {
    if (!data[0].canConvert<QSharedPointer<Pokemon>>()) {
        qDebug() << "ERROR: Cannot convert QVariant to Pokemon";
    }
    QSharedPointer<Pokemon> pokemon = data[0].value<QSharedPointer<Pokemon>>();
    return QSharedPointer<SummaryPage>::create(pokemon);
}
