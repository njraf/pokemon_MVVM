#include "battlepage.h"
#include "ui_battlepage.h"

BattlePage::BattlePage(QSharedPointer<BattleViewmodel> vm_, QWidget *parent)
    : IPage(parent)
    , ui(new Ui::BattlePage)
    , viewmodel(vm_)
{
    ui->setupUi(this);

    setObjectName("BattlePage");

    connect(ui->fightButton, &QPushButton::clicked, this, [=] { emit changedPage(PageName::BAG); });
    connect(ui->runButton, &QPushButton::clicked, this, [=] { emit returnedPage(); });

    auto playerTeam = viewmodel->getPlayerTrainer()->getTeam();
    auto pokeIt = std::find_if(playerTeam.begin(), playerTeam.end(), [](QSharedPointer<Pokemon> pokemon) { return (pokemon->getHealthStat() > 0); });
    if (pokeIt != playerTeam.end()) {
        ui->allyName->setText((*pokeIt)->getName());
        ui->allyHp->setText(QString("%1/%2").arg((*pokeIt)->getHealthStat()).arg((*pokeIt)->getMaxHealthStat()));
        ui->allyHpBar->setMaximum((*pokeIt)->getMaxHealthStat());
        ui->allyHpBar->setValue((*pokeIt)->getHealthStat());
        ui->allyLevel->setText(QString("L: %1").arg((*pokeIt)->getLevel()));
    }

    auto opponentTeam = viewmodel->getOpponentTrainer()->getTeam();
    pokeIt = std::find_if(opponentTeam.begin(), opponentTeam.end(), [](QSharedPointer<Pokemon> pokemon) { return (pokemon->getHealthStat() > 0); });
    if (pokeIt != opponentTeam.end()) {
        ui->opponentName->setText((*pokeIt)->getName());
        ui->opponentHp->setText(QString("%1/%2").arg((*pokeIt)->getHealthStat()).arg((*pokeIt)->getMaxHealthStat()));
        ui->opponentHpBar->setMaximum((*pokeIt)->getMaxHealthStat());
        ui->opponentHpBar->setValue((*pokeIt)->getHealthStat());
        ui->opponentLevel->setText(QString("L: %1").arg((*pokeIt)->getLevel()));
    }
}

BattlePage::~BattlePage()
{
    delete ui;
}

PageName BattlePage::getPageName() {
    return PageName::BATTLE;
}
