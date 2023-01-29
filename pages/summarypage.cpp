#include "summarypage.h"
#include "ui_summarypage.h"
#include "pagenavigator.h"

SummaryPage::SummaryPage(QWidget *parent)
    : IPage(parent)
    , ui(new Ui::SummaryPage)
{
    ui->setupUi(this);
    connect(ui->backButton, &QPushButton::clicked, this, [=] { PageNavigator::getInstance()->navigateBack(); });


}

SummaryPage::~SummaryPage()
{
    delete ui;
}

PageName SummaryPage::getPageName() {
    return PageName::POKEMON_SUMMARY;
}

void SummaryPage::receiveData(QVector<QVariant> data) {
    if (data.isEmpty() || !data[0].canConvert<QSharedPointer<Pokemon>>()) {
        qDebug() << "ERROR: Cannot convert QVariant to Pokemon";
        return;
    }
    pokemon = data[0].value<QSharedPointer<Pokemon>>();
    //ui->label->setText(pokemon->getName());
    auto attackList = pokemon->getAttackList();
    if (attackList.size() >= 1)
        ui->attack1->setText(attackList[0]->getName());
    if (attackList.size() >= 2)
        ui->attack2->setText(attackList[1]->getName());
    if (attackList.size() >= 3)
        ui->attack3->setText(attackList[2]->getName());
    if (attackList.size() >= 4)
        ui->attack4->setText(attackList[3]->getName());
}
