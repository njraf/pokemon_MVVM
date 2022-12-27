#include "mainmenupage.h"
#include "ui_mainmenupage.h"
#include "pagenavigator.h"

MainMenuPage::MainMenuPage(QWidget *parent)
    : IPage(parent)
    , ui(new Ui::MainMenuPage)
{
    ui->setupUi(this);

    connect(ui->battleButton, &QPushButton::clicked, this, [=] { PageNavigator::getInstance()->navigate(PageName::BATTLE); });
    connect(ui->bagButton, &QPushButton::clicked, this, [=] { PageNavigator::getInstance()->navigate(PageName::BAG); });
    connect(ui->pokemonButton, &QPushButton::clicked, this, [=] { PageNavigator::getInstance()->navigate(PageName::TEAM); });
}

MainMenuPage::~MainMenuPage()
{
    delete ui;
}


PageName MainMenuPage::getPageName() {
    return PageName::MAIN_MENU;
}

void MainMenuPage::receiveData(QVector<QVariant> data) {

}
