#include "teampage.h"
#include "ui_teampage.h"
#include "pagenavigator.h"

TeamPage::TeamPage(QVector<QSharedPointer<Pokemon>> team_, QWidget *parent)
    : IPage(parent)
    , ui(new Ui::TeamPage)
    , team(team_)
    , state(InteractionState::NONE)
{
    ui->setupUi(this);
    connect(ui->backButton, &QPushButton::clicked, this, [=] { PageNavigator::getInstance()->navigateBack(); });

    auto routeNames = PageNavigator::getInstance()->getRouteNames();
    switch (routeNames.at(routeNames.size() - 1)) {
    case PageName::BATTLE:
        state = InteractionState::BATTLE;
        break;
    case PageName::MAIN_MENU:
        state = InteractionState::MAIN_MENU;
        break;
    default:
        state = InteractionState::NONE;
        break;
    }

    int row = 0;
    int col = 0;
    for (auto member : team) {
        TeamMemberCard *teamMemberCard = new TeamMemberCard(member);
        ui->teamGrid->addWidget(teamMemberCard, row, col);
        connect(teamMemberCard, &TeamMemberCard::clicked, this, [=] {
            QVector<QVariant> data = {QVariant::fromValue<QSharedPointer<Pokemon>>(member)};
            if (state == InteractionState::BATTLE) {
                if ((battlePokemon->getName() == member->getName()) && (battlePokemon->getLevel() == member->getLevel())) {
                    // selected pokemon is not the current pokemon in battle
                    //TODO: set better condition
                    qDebug() << "Cannot send out the same pokemon";
                } else if (member->getHealthStat() <= 0) {
                    qDebug() << "That pokemon has no HP left";
                } else {
                    PageNavigator::getInstance()->navigateBack(data);
                }
            } else if (state == InteractionState::MAIN_MENU) {
                PageNavigator::getInstance()->navigate(PageName::POKEMON_SUMMARY, data);
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
    for (int i = team.size(); i < 6; i++) {
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
    if (data.isEmpty() || !data[0].canConvert<QSharedPointer<Pokemon>>()) {
        return;
    }

    if (state == InteractionState::BATTLE) {
        battlePokemon = data[0].value<QSharedPointer<Pokemon>>();
    }
}
