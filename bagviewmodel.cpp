#include "bagviewmodel.h"

BagViewmodel::BagViewmodel(QSharedPointer<Bag> bag_, QObject *parent)
    : QObject(parent)
    , bag(bag_)
{

}

void BagViewmodel::addHealItem(QSharedPointer<HealItem> item) {
    bag->addHealItem(item);
}

QVector<QSharedPointer<HealItem>> BagViewmodel::getHealItems() {
    return bag->getHealItems();
}
