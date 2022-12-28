#include "bag.h"

Bag::Bag(QObject *parent) : QObject(parent)
{

}

void Bag::addHealItem(QSharedPointer<HealItem> item) {
    healItems.append(item);
}

QVector<QSharedPointer<HealItem>> Bag::getHealItems() {
    return healItems;
}
