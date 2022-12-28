#ifndef BAGPAGE_H
#define BAGPAGE_H

#include <QWidget>
#include <QSharedPointer>

#include "bagviewmodel.h"
#include "ipage.h"

namespace Ui {
class BagPage;
}

class BagPage : public IPage
{
    Q_OBJECT

public:
    explicit BagPage(QSharedPointer<BagViewmodel> viewmodel_, QWidget *parent = nullptr);
    ~BagPage();

    PageName getPageName() override;
    void receiveData(QVector<QVariant> data) override;
    void displayItems();

private:
    enum class Context : int {
        NONE,
        TEAM, // from TeamPage
        MENU  // from main menu or battle
    };
    Ui::BagPage *ui;
    QSharedPointer<BagViewmodel> viewmodel;
    Context context;
};

#endif // BAGPAGE_H
