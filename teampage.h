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
    Ui::TeamPage *ui;
    QVector<QSharedPointer<Pokemon>> team;
};

#endif // TEAMPAGE_H
