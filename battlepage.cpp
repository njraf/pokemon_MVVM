#include "battlepage.h"
#include "ui_battlepage.h"

BattlePage::BattlePage(QSharedPointer<BattleViewmodel> vm_, QWidget *parent)
    : IPage(parent)
    , ui(new Ui::BattlePage)
    , viewmodel(vm_)
{
    ui->setupUi(this);

    setObjectName("BattlePage");

    connect(ui->pushButton, &QPushButton::clicked, this, [&] { emit changePage(PageName::BAG); }); // sample

    auto playerTeam = viewmodel->getPlayerTrainer()->getTeam();
    if (!playerTeam.isEmpty()) {
        ui->allyName->setText(playerTeam[0]->getName());
    }

    auto opponentTeam = viewmodel->getOpponentTrainer()->getTeam();
    if (!opponentTeam.isEmpty()) {
        ui->opponentName->setText(opponentTeam[0]->getName());
    }
}

BattlePage::~BattlePage()
{
    delete ui;
}

PageName BattlePage::getPageName() {
    return PageName::BATTLE;
}
