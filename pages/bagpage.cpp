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

    auto routeNames = PageNavigator::getInstance()->getRouteNames();
    switch (routeNames.at(routeNames.size() - 1)) {
    case PageName::TEAM:
        context = Context::TEAM;
        break;
    case PageName::MAIN_MENU:
    case PageName::BATTLE:
        context = Context::MENU;
        break;
    default:
        context = Context::NONE;
        break;
    }


    connect(ui->backButton, &QPushButton::clicked, this, [=] { PageNavigator::getInstance()->navigateBack(); });
    connect(ui->healItemList, &QListWidget::itemEntered, this, [=](QListWidgetItem *listItem) {
        auto healItems = viewmodel->getHealItems();
        QString text = listItem->text();
        QString itemName = text.split('\t').last();
        auto itemIt = std::find_if(healItems.begin(), healItems.end(), [=](QSharedPointer<HealItem> healItem) { return (healItem->getName() == itemName); });
        if (itemIt != healItems.end()) {
            ui->itemDescriptionLabel->setText((*itemIt)->getDescription());
        }
    });

    connect(ui->healItemList, &QListWidget::itemClicked, this, [=](QListWidgetItem *listItem) {
        auto healItems = viewmodel->getHealItems();
        QString text = listItem->text();
        QString itemName = text.split('\t').last();
        auto itemIt = std::find_if(healItems.begin(), healItems.end(), [=](QSharedPointer<HealItem> healItem) { return (healItem->getName() == itemName); });
        if (itemIt == healItems.end()) {
            return;
        }

        if (context == Context::MENU) {
            QVector<QVariant> data;
            data.append(QVariant::fromValue<QSharedPointer<HealItem>>(*itemIt));
            PageNavigator::getInstance()->navigate(PageName::TEAM, data);
        }

    });

}

BagPage::~BagPage()
{
    delete ui;
}

PageName BagPage::getPageName() {
    return PageName::BAG;
}

void BagPage::receiveData(QVector<QVariant> data) {
    displayItems();
}

void BagPage::displayItems() {
    auto healItems = viewmodel->getHealItems();
    QStringList names;
    for (auto item : healItems) {
        names.append(QString("x%1\t%2").arg(item->getQuantity()).arg(item->getName()));
    }
    ui->healItemList->clear();
    ui->healItemList->addItems(names);
}
