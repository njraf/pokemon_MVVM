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

private:
    Ui::BagPage *ui;
    QSharedPointer<BagViewmodel> viewmodel;
};

#endif // BAGPAGE_H
