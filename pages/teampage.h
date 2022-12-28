#ifndef TEAMPAGE_H
#define TEAMPAGE_H

#include <QWidget>

#include "ipage.h"
#include "pokemon.h"
#include "teammembercard.h"
#include "healitem.h"

/*
 * battle state:
 * shift (into battle)
 * summary
 * cancel
 *
 * menu state:
 * summary
 * switch (places in party)
 * item
 *   give
 *   take
 * cancel
 */

namespace Ui {
class TeamPage;
}

class TeamPage : public IPage
{
    Q_OBJECT

public:
    explicit TeamPage(QVector<QSharedPointer<Pokemon>> team_, QWidget *parent = nullptr);
    ~TeamPage();
    PageName getPageName() override;
    void receiveData(QVector<QVariant> data) override;

private:
    enum class Context : int { // specifies the context this page was opened under (e.g. in battle or from the main menu)
        NONE, BATTLE, MAIN_MENU, BAG
    };
    Ui::TeamPage *ui;
    QVector<QSharedPointer<Pokemon>> team;
    Context context;
    QSharedPointer<Pokemon> battlePokemon;
    QSharedPointer<HealItem> healItemToUse;
};

#endif // TEAMPAGE_H
