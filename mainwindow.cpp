#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QSharedPointer<BattleViewmodel> vm_, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , viewmodel(vm_)
{
    ui->setupUi(this);

    auto playerTeam = viewmodel->getPlayerTrainer()->getTeam();
    if (!playerTeam.isEmpty()) {
        ui->allyName->setText(playerTeam[0]->getName());
    }

    auto opponentTeam = viewmodel->getOpponentTrainer()->getTeam();
    if (!opponentTeam.isEmpty()) {
        ui->opponentName->setText(opponentTeam[0]->getName());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

