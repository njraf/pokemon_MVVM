#include "bagpage.h"
#include "ui_bagpage.h"
#include "pagenavigator.h"

#include <QStringList>


BagPage::BagPage(QSharedPointer<BagViewmodel> viewmodel_, QWidget *parent) :
    IPage(parent),
    ui(new Ui::BagPage),
    viewmodel(viewmodel_)
{
    ui->setupUi(this);

    auto healItems = viewmodel->getHealItems();
    QStringList names;
    for (auto item : healItems) {
        names.append(QString("x%1\t%2").arg(item->getQuantity()).arg(item->getName()));
    }
    ui->healItemList->addItems(names);

    connect(ui->healItemList, &QListWidget::itemEntered, this, [=](QListWidgetItem *listItem) {
        QString text = listItem->text();
        QString itemName = text.split('\t').last();
        auto itemIt = std::find_if(healItems.begin(), healItems.end(), [=](QSharedPointer<HealItem> healItem) { return (healItem->getName() == itemName); });
        if (itemIt != healItems.end()) {
            ui->itemDescriptionLabel->setText((*itemIt)->getDescription());
        }
    });

    connect(ui->backButton, &QPushButton::clicked, this, [=] { PageNavigator::getInstance()->navigateBack(); });
}

BagPage::~BagPage()
{
    delete ui;
}

PageName BagPage::getPageName() {
    return PageName::BAG;
}

void BagPage::receiveData(QVector<QVariant> data) {

}
