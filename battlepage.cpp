#include "battlepage.h"
#include "ui_battlepage.h"

#include <QMessageBox>

BattlePage::BattlePage(QSharedPointer<BattleViewmodel> vm_, QWidget *parent)
    : IPage(parent)
    , ui(new Ui::BattlePage)
    , viewmodel(vm_)
{
    ui->setupUi(this);

    setObjectName("BattlePage");

    auto playerTeam = viewmodel->getPlayerTrainer()->getTeam();
    auto opponentTeam = viewmodel->getOpponentTrainer()->getTeam();
    bool playerKOed = (0 == std::count_if(playerTeam.begin(), playerTeam.end(), [](QSharedPointer<Pokemon> pokemon) { return (pokemon->getHealthStat() > 0); }));
    bool opponentKOed = (0 == std::count_if(opponentTeam.begin(), opponentTeam.end(), [](QSharedPointer<Pokemon> pokemon) { return (pokemon->getHealthStat() > 0); }));
    if (playerKOed) {
        qDebug() << "ERROR: You have no usable pokemon";
    } else if (opponentKOed) {
        qDebug() << "ERROR: The opponent has no usable pokemon";
    }

    // action buttons
    connect(ui->fightButton, &QPushButton::clicked, this, [=] { ui->actionArea->setCurrentIndex(1); });
    connect(ui->bagButton, &QPushButton::clicked, this, [=] { emit changedPage(PageName::BAG); });
    connect(ui->pokemonButton, &QPushButton::clicked, this, [=] { emit changedPage(PageName::TEAM); });
    connect(ui->runButton, &QPushButton::clicked, this, [=] { emit returnedPage(); });

    // attack buttons
    connect(ui->backButton, &QPushButton::clicked, this, [=] { ui->actionArea->setCurrentIndex(0); });
    for (int attackIndex = 0; attackIndex < 4; attackIndex++) {
        int row = ((attackIndex % 2) == 0) ? 0 : 1;
        int col = (attackIndex >= 2) ? 2 : 1;
        auto layoutItem = qobject_cast<QGridLayout*>(ui->attackPage->layout())->itemAtPosition(row, col);
        if (!layoutItem || !layoutItem->widget()) {
            continue;
        }
        auto button = qobject_cast<QPushButton*>(layoutItem->widget());
        connect(button, &QPushButton::clicked, this, [=] { viewmodel->attack(attackIndex); });
    }

    // viewmodel signals
    connect(viewmodel.data(), &BattleViewmodel::summonedPokemon, this, &BattlePage::displayStats);
    connect(viewmodel.data(), &BattleViewmodel::stateUpdated, this, [=] { displayStats(viewmodel->getCurrentPlayerPokemon(), viewmodel->getCurrentOpponentPokemon()); });
    connect(viewmodel.data(), &BattleViewmodel::stateUpdated, this, [=] {
        // determine win/lose
        auto playerTeam = viewmodel->getPlayerTrainer()->getTeam();
        auto opponentTeam = viewmodel->getOpponentTrainer()->getTeam();
        bool playerKOed = (0 == std::count_if(playerTeam.begin(), playerTeam.end(), [](QSharedPointer<Pokemon> pokemon) { return (pokemon->getHealthStat() > 0); }));
        bool opponentKOed = (0 == std::count_if(opponentTeam.begin(), opponentTeam.end(), [](QSharedPointer<Pokemon> pokemon) { return (pokemon->getHealthStat() > 0); }));

        QString winLoseMessage = "";
        if (playerKOed) {
            winLoseMessage = "You lost";
        } else if (opponentKOed) {
            winLoseMessage = "You won";
        } else { // no KOs
            return;
        }

        QMessageBox winLoseDialog;
        winLoseDialog.setText(winLoseMessage);
        winLoseDialog.exec();
        emit returnedPage();
    });

    if (!playerKOed && !opponentKOed) {
        viewmodel->summonFirstPokemon();
    }
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
    for (int attackIndex = 0; attackIndex < playerPokemon->getAttackList().size(); attackIndex++) {
        int row = ((attackIndex % 2) == 0) ? 0 : 1;
        int col = (attackIndex >= 2) ? 2 : 1;
        auto layoutItem = qobject_cast<QGridLayout*>(ui->attackPage->layout())->itemAtPosition(row, col);
        if (!layoutItem || !layoutItem->widget()) {
            continue;
        }
        auto button = qobject_cast<QPushButton*>(layoutItem->widget());
        auto attack = playerPokemon->getAttackList().at(attackIndex);
        QString attackName = attack->getName();
        QString ppLine = QString("%1/%2").arg(attack->getPp()).arg(attack->getMaxPP());
        button->setText(attackName + "\n" + ppLine);
    }

    ui->opponentName->setText(opponentPokemon->getName());
    ui->opponentHp->setText(QString("%1/%2").arg(opponentPokemon->getHealthStat()).arg(opponentPokemon->getMaxHealthStat()));
    ui->opponentHpBar->setMaximum(opponentPokemon->getMaxHealthStat());
    ui->opponentHpBar->setValue(opponentPokemon->getHealthStat());
    ui->opponentLevel->setText(QString("L: %1").arg(opponentPokemon->getLevel()));

    ui->actionArea->setCurrentIndex(0);
}



