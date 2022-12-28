#include "summarypage.h"
#include "ui_summarypage.h"
#include "pagenavigator.h"

SummaryPage::SummaryPage(QSharedPointer<Pokemon> pokemon_, QWidget *parent)
    : IPage(parent)
    , ui(new Ui::SummaryPage)
    , pokemon(pokemon_)
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
    ui->label->setText(pokemon->getName());
}
