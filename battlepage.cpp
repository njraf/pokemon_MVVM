#include "battlepage.h"
#include "ui_battlepage.h"

BattlePage::BattlePage(QSharedPointer<BattleViewmodel> vm_, QWidget *parent)
    : IPage(parent)
    , ui(new Ui::BattlePage)
    , viewmodel(vm_)
{
    ui->setupUi(this);

    setObjectName("BattlePage");

    connect(ui->fightButton, &QPushButton::clicked, this, [=] { ui->actionArea->setCurrentIndex(1); });
    connect(ui->backButton, &QPushButton::clicked, this, [=] { ui->actionArea->setCurrentIndex(0); });
    connect(ui->fightButton, &QPushButton::clicked, this, [=] { emit changedPage(PageName::BAG); });
    connect(ui->pokemonButton, &QPushButton::clicked, this, [=] { emit changedPage(PageName::TEAM); });
    connect(ui->runButton, &QPushButton::clicked, this, [=] { emit returnedPage(); });
    connect(viewmodel.data(), &BattleViewmodel::summonedPokemon, this, &BattlePage::displayStats);

    viewmodel->summonFirstPokemon();
}

BattlePage::~BattlePage()
{
    delete ui;
}

PageName BattlePage::getPageName() {
    return PageName::BATTLE;
}

void BattlePage::displayStats(QSharedPointer<Pokemon> playerPokemon, QSharedPointer<Pokemon> opponentPokemon) {
    ui->allyName->setText(playerPokemon->getName());
    ui->allyHp->setText(QString("%1/%2").arg(playerPokemon->getHealthStat()).arg(playerPokemon->getMaxHealthStat()));
    ui->allyHpBar->setMaximum(playerPokemon->getMaxHealthStat());
    ui->allyHpBar->setValue(playerPokemon->getHealthStat());
    ui->allyLevel->setText(QString("L: %1").arg(playerPokemon->getLevel()));

    ui->opponentName->setText(opponentPokemon->getName());
    ui->opponentHp->setText(QString("%1/%2").arg(opponentPokemon->getHealthStat()).arg(opponentPokemon->getMaxHealthStat()));
    ui->opponentHpBar->setMaximum(opponentPokemon->getMaxHealthStat());
    ui->opponentHpBar->setValue(opponentPokemon->getHealthStat());
    ui->opponentLevel->setText(QString("L: %1").arg(opponentPokemon->getLevel()));
}



