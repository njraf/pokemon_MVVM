#ifndef SUMMARYPAGE_H
#define SUMMARYPAGE_H

#include "ipage.h"
#include "pokemon.h"

#include <QWidget>

namespace Ui {
class SummaryPage;
}

class SummaryPage : public IPage
{
    Q_OBJECT

public:
    explicit SummaryPage(QWidget *parent = nullptr);
    ~SummaryPage();
    PageName getPageName() override;
    void receiveData(QVector<QVariant> data) override;

private:
    Ui::SummaryPage *ui;
    QSharedPointer<Pokemon> pokemon;

};

#endif // SUMMARYPAGE_H
