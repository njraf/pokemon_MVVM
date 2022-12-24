#include "mainmenupage.h"
#include "ui_mainmenupage.h"

MainMenuPage::MainMenuPage(QWidget *parent)
    : IPage(parent)
    , ui(new Ui::MainMenuPage)
{
    ui->setupUi(this);

    connect(ui->battleButton, &QPushButton::clicked, this, [=] { emit changedPage(PageName::BATTLE); });
    connect(ui->bagButton, &QPushButton::clicked, this, [=] { emit changedPage(PageName::BAG); });
    connect(ui->pokemonButton, &QPushButton::clicked, this, [=] { emit changedPage(PageName::TEAM); });
    connect(ui->exitButton, &QPushButton::clicked, this, [=] { emit quit(); });
}

MainMenuPage::~MainMenuPage()
{
    delete ui;
}


PageName MainMenuPage::getPageName() {
    return PageName::MAIN_MENU;
}
