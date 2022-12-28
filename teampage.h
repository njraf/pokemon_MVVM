#ifndef TEAMPAGE_H
#define TEAMPAGE_H

#include <QWidget>

#include "ipage.h"
#include "pokemon.h"
#include "teammembercard.h"

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
    enum class InteractionState : int { // specifies the context this page was opened under (e.g. in battle or from the main menu)
        NONE, BATTLE, MAIN_MENU
    };
    Ui::TeamPage *ui;
    QVector<QSharedPointer<Pokemon>> team;
    InteractionState state;
    QSharedPointer<Pokemon> battlePokemon;
};

#endif // TEAMPAGE_H
