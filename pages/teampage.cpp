#include "teampage.h"
#include "ui_teampage.h"
#include "pagenavigator.h"

#include <QMessageBox>

TeamPage::TeamPage(QSharedPointer<QVector<QSharedPointer<Pokemon>>> team_, QWidget *parent)
    : IPage(parent)
    , ui(new Ui::TeamPage)
    , team(team_)
    , context(Context::NONE)
{
    ui->setupUi(this);
    connect(ui->backButton, &QPushButton::clicked, this, [=] { PageNavigator::getInstance()->navigateBack(); });

    auto routeNames = PageNavigator::getInstance()->getRouteNames();
    switch (routeNames.at(routeNames.size() - 1)) {
    case PageName::BATTLE:
        context = Context::BATTLE;
        break;
    case PageName::MAIN_MENU:
        context = Context::MAIN_MENU;
        break;
    case PageName::BAG:
        context = Context::BAG;
        break;
    case PageName::OVERWORLD:
        context = Context::OVERWORLD;
        break;
    default:
        context = Context::NONE;
        break;
    }

    int row = 0;
    int col = 0;
    for (auto member : *team) {
        TeamMemberCard *teamMemberCard = new TeamMemberCard(member);
        ui->teamGrid->addWidget(teamMemberCard, row, col);
        connect(teamMemberCard, &TeamMemberCard::clicked, this, [=] {
            QVector<QVariant> data = {QVariant::fromValue<QSharedPointer<Pokemon>>(member)};
            if (context == Context::BAG) {
                if ((member->getHealthStat() != 0) && (member->getHealthStat() < member->getMaxHealthStat())) {
                    //TODO: assumes HealItem. Allow actions from other items.
                    healItemToUse->use(member);
                    PageNavigator::getInstance()->navigateBack();
                } else {
                    qDebug() << "Cannot heal this pokemon";
                }
            } else if (context == Context::BATTLE) {
                showBattleDialog(member);
            } else if ((context == Context::MAIN_MENU) || (context == Context::OVERWORLD)) {
                showMenuDialog(member);
            }
        });

        if ((col % 2) == 0) {
            col++;
        } else {
            col--;
            row++;
        }
    }

    // fill remaining slots with an empty widget
    for (int i = team->size(); i < 6; i++) {
        ui->teamGrid->addWidget(new QWidget(), row, col);

        if ((col % 2) == 0) {
            col++;
        } else {
            col--;
            row++;
        }
    }
}

TeamPage::~TeamPage()
{
    delete ui;
}

PageName TeamPage::getPageName() {
    return PageName::TEAM;
}

void TeamPage::receiveData(QVector<QVariant> data) {
    if (data.isEmpty()) {
        return;
    }

    if (context == Context::BATTLE && data[0].canConvert<QSharedPointer<Pokemon>>()) {
        battlePokemon = data[0].value<QSharedPointer<Pokemon>>();
    } else if (context == Context::BAG && data[0].canConvert<QSharedPointer<HealItem>>()) {
        healItemToUse = data[0].value<QSharedPointer<HealItem>>();
    }
}

void TeamPage::showMenuDialog(QSharedPointer<Pokemon> pokemon) {
    QVariant pokeData = QVariant::fromValue<QSharedPointer<Pokemon>>(pokemon);
    QVector<QVariant> data = {pokeData};
    QPushButton *summaryButton = new QPushButton("Summary");
    QPushButton *switchButton = new QPushButton("Switch");
    QPushButton *itemButton = new QPushButton("Item");
    QPushButton *cancelButton = new QPushButton("Cancel");

    QMessageBox dialog;
    QLayout *layout = dialog.layout();
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        delete item;
    }
    layout->addWidget(summaryButton);
    layout->addWidget(switchButton);
    layout->addWidget(itemButton);
    layout->addWidget(cancelButton);


    dialog.setLayout(layout);

    connect(summaryButton, &QPushButton::clicked, this, [&] { dialog.reject(); PageNavigator::getInstance()->navigate(PageName::POKEMON_SUMMARY, data); });
    connect(switchButton, &QPushButton::clicked, this, [&] {  });
    connect(itemButton, &QPushButton::clicked, this, [&] {  });
    connect(cancelButton, &QPushButton::clicked, this, [&] { dialog.reject(); });

    dialog.exec();
}

void TeamPage::showBattleDialog(QSharedPointer<Pokemon> pokemon) {
    QVariant pokeData = QVariant::fromValue<QSharedPointer<Pokemon>>(pokemon);
    QVector<QVariant> data = {pokeData};
    QPushButton *shiftButton = new QPushButton("Shift");
    QPushButton *summaryButton = new QPushButton("Summary");
    QPushButton *cancelButton = new QPushButton("Cancel");

    QMessageBox dialog;
    QLayout *layout = dialog.layout();
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        delete item;
    }
    layout->addWidget(shiftButton);
    layout->addWidget(summaryButton);
    layout->addWidget(cancelButton);


    dialog.setLayout(layout);

    connect(shiftButton, &QPushButton::clicked, this, [&] {
        if ((battlePokemon->getName() == pokemon->getName()) && (battlePokemon->getLevel() == pokemon->getLevel())) {
            // selected pokemon is not the current pokemon in battle
            //TODO: set better condition
            qDebug() << battlePokemon->getName() << "is already in battle";
        } else if (pokemon->getHealthStat() <= 0) {
            qDebug() << "That pokemon has no HP left";
        } else {
            dialog.reject();
            PageNavigator::getInstance()->navigateBack(data);
        }
    });
    connect(summaryButton, &QPushButton::clicked, this, [&] { dialog.reject(); PageNavigator::getInstance()->navigate(PageName::POKEMON_SUMMARY, data); });
    connect(cancelButton, &QPushButton::clicked, this, [&] { dialog.reject(); });

    dialog.exec();
}
