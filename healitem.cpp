#include "healitem.h"

#include <QDebug>

HealItem::HealItem(QString name_, int healAmount_, int quantity_, QString description_, QObject *parent)
    : QObject(parent)
    , name(name_)
    , healAmount(healAmount_)
    , quantity(quantity_)
    , description(description_)
{

}

QString HealItem::getName() {
    return name;
}

int HealItem::getHealAmount() {
    return healAmount;
}

int HealItem::getQuantity() {
    return quantity;
}

void HealItem::setQuantity(int quantity_) {
    quantity = quantity_;
    if (quantity <= 0) {
        emit usedLast();
    }
}

QString HealItem::getDescription() {
    return description;
}

void HealItem::use(QSharedPointer<Pokemon> pokemon) {
    qDebug() << "Used a" << name << "on" << pokemon->getName();
    pokemon->setHealthStat(pokemon->getHealthStat() + healAmount);
    setQuantity(--quantity);
}
