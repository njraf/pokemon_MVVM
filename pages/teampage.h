#ifndef TEAMPAGE_H
#define TEAMPAGE_H

#include <QWidget>

#include "ipage.h"
#include "pokemon.h"
#include "teammembercard.h"
#include "healitem.h"

/*
 * battle context:
 * shift (into battle)
 * summary
 * cancel
 *
 * menu context:
 * summary
 * switch (places in party)
 * item
 *   give
 *   take
 * cancel
 *
 * bag context:
 * use/give item
 */

namespace Ui {
class TeamPage;
}

class TeamPage : public IPage
{
    Q_OBJECT

public:
    explicit TeamPage(QSharedPointer<QVector<QSharedPointer<Pokemon>>> team_, QWidget *parent = nullptr);
    ~TeamPage();
    PageName getPageName() override;
    void receiveData(QVector<QVariant> data) override;
    void showMenuDialog(QSharedPointer<Pokemon> pokemon, TeamMemberCard *teamMemberCard);
    void showBattleDialog(QSharedPointer<Pokemon> pokemon);

private:
    enum class Context : int { // specifies the context this page was opened under (e.g. in battle or from the main menu)
        NONE, BATTLE, MAIN_MENU, BAG, OVERWORLD
    };
    Ui::TeamPage *ui;
    QSharedPointer<QVector<QSharedPointer<Pokemon>>> team;
    Context context;
    QSharedPointer<Pokemon> battlePokemon;
    QSharedPointer<HealItem> healItemToUse;
    QSharedPointer<Pokemon> selectedPokemon; // selection for switchMode
    bool switchMode; // indicates switching party order

    void drawPage();
};

#endif // TEAMPAGE_H
