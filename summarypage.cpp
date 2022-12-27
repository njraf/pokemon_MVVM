#include "summarypage.h"
#include "ui_summarypage.h"

SummaryPage::SummaryPage(QSharedPointer<Pokemon> pokemon_, QWidget *parent)
    : IPage(parent)
    , ui(new Ui::SummaryPage)
    , pokemon(pokemon_)
{
    ui->setupUi(this);
    qDebug() << pokemon->getName();
}

SummaryPage::~SummaryPage()
{
    delete ui;
}

PageName SummaryPage::getPageName() {
    return PageName::POKEMON_SUMMARY;
}

void SummaryPage::receiveData(QVector<QVariant> data) {

}
