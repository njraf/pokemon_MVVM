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
    case PageName::OVERWORLD:
        context = Context::MENU;
        break;
    case PageName::BATTLE:
        context = Context::BATTLE;
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

        if ((context == Context::MENU) || (context == Context::BATTLE)) {
            QVector<QVariant> data;
            data.append(QVariant::fromValue<QSharedPointer<HealItem>>(*itemIt));
            PageNavigator::getInstance()->navigate(PageName::TEAM, data);
        }

    });

    connect(ui->pokeballItemList, &QListWidget::itemEntered, this, [=](QListWidgetItem *listItem) {
        auto pokeballItems = viewmodel->getPokeballItems();
        QString text = listItem->text();
        QString itemName = text.split('\t').last();
        auto itemIt = std::find_if(pokeballItems.begin(), pokeballItems.end(), [=](QSharedPointer<PokeballItem> pokeballItem) { return (pokeballItem->getName() == itemName); });
        if (itemIt != pokeballItems.end()) {
            ui->itemDescriptionLabel->setText(itemName);
        }
    });

    connect(ui->pokeballItemList, &QListWidget::itemClicked, this, [=](QListWidgetItem *listItem) {
        if (context == Context::BATTLE) { // using pokeball in battle. attempt catch.
            auto pokeballItems = viewmodel->getPokeballItems();
            QString text = listItem->text();
            QString itemName = text.split('\t').last();
            auto itemIt = std::find_if(pokeballItems.begin(), pokeballItems.end(), [=](QSharedPointer<PokeballItem> pokeballItem) { return (pokeballItem->getName() == itemName); });
            if (itemIt == pokeballItems.end()) {
                return;
            }

            QVector<QVariant> data;
            data.append(QVariant::fromValue<QSharedPointer<PokeballItem>>(*itemIt));
            PageNavigator::getInstance()->navigateBack(data);
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
    // heal items
    auto healItems = viewmodel->getHealItems();
    QStringList names;
    for (auto item : healItems) {
        names.append(QString("x%1\t%2").arg(item->getQuantity()).arg(item->getName()));
    }
    ui->healItemList->clear();
    ui->healItemList->addItems(names);
    names.clear();

    // pokeballs
    auto pokeballItems = viewmodel->getPokeballItems();
    for (auto item : pokeballItems) {
        names.append(QString("x%1\t%2").arg(item->getQuantity()).arg(item->getName()));
    }
    ui->pokeballItemList->clear();
    ui->pokeballItemList->addItems(names);
    names.clear();
}
