#include "teampage.h"
#include "ui_teampage.h"
#include "pagenavigator.h"

TeamPage::TeamPage(QVector<QSharedPointer<Pokemon>> team_, QWidget *parent)
    : IPage(parent)
    , ui(new Ui::TeamPage)
    , team(team_)
{
    ui->setupUi(this);
    connect(ui->backButton, &QPushButton::clicked, this, [=] { PageNavigator::getInstance()->navigateBack(); });

    auto routeNames = PageNavigator::getInstance()->getRouteNames();

    int row = 0;
    int col = 0;
    for (auto member : team) {
        TeamMemberCard *teamMemberCard = new TeamMemberCard(member);
        ui->teamGrid->addWidget(teamMemberCard, row, col);
        connect(teamMemberCard, &TeamMemberCard::clicked, this, [=] {
            if (!routeNames.isEmpty()) {
                QVector<QVariant> data = {QVariant::fromValue<QSharedPointer<Pokemon>>(member)};
                auto prevPageName = routeNames.at(routeNames.size() - 1);
                if (prevPageName == PageName::BATTLE) {
                    PageNavigator::getInstance()->navigateBack(data);
                } else if (prevPageName == PageName::MAIN_MENU) {
                    PageNavigator::getInstance()->navigate(PageName::POKEMON_SUMMARY, data);
                }
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

}
