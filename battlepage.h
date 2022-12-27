#ifndef BATTLEPAGE_H
#define BATTLEPAGE_H

#include <QWidget>
#include <QSharedPointer>

#include "battleviewmodel.h"
#include "ipage.h"

namespace Ui {
class BattlePage;
}

class BattlePage : public IPage
{
    Q_OBJECT

public:
    explicit BattlePage(QSharedPointer<BattleViewmodel> vm_, QWidget *parent = nullptr);
    ~BattlePage();
    PageName getPageName() override;
    void receiveData(QVector<QVariant> data) override;

private:
    Ui::BattlePage *ui;
    QSharedPointer<BattleViewmodel> viewmodel;

public slots:
    void displayStats(QSharedPointer<Pokemon> playerPokemon, QSharedPointer<Pokemon> opponentPokemon);
};

#endif // BATTLEPAGE_H
