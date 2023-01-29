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

void Bag::addPokeballItem(QSharedPointer<PokeballItem> item) {
    pokeballItems.append(item);
}

QVector<QSharedPointer<PokeballItem>> Bag::getPokeballItems() {
    return pokeballItems;
}
