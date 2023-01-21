#include "pokeballitem.h"

#include <QtMath>
#include <QRandomGenerator>
#include <QDebug>

PokeballItem::PokeballItem(QString name_, double catchRate_, int quantity_, QObject *parent)
    : QObject(parent)
    , name(name_)
    , catchRate(catchRate_)
    , quantity(quantity_)
{

}

QString PokeballItem::getName() {
    return name;
}

int PokeballItem::getQuantity() {
    return quantity;
}

void PokeballItem::setQuantity(int quantity_) {
    quantity = quantity_;
}

bool PokeballItem::throwAtPokemon(QSharedPointer<Pokemon> pokemon) {
    int maxHealth = pokemon->getMaxHealthStat();
    int catchChance = (3 * maxHealth - 2 * pokemon->getHealthStat()) * pokemon->getCatchRate() * catchRate;
    catchChance /= (3 * pokemon->getMaxHealthStat());
    catchChance = ((36 - 2 * pokemon->getLevel()) * catchChance) / 10; // make it easier to cathc pokemon level 13 and under
    catchChance *= pokemon->getStatusCondition()->catchMultiplier(); // status condition multiplier
    uint shakeChance = 1048560 / qSqrt(qSqrt(16711680 / catchChance));
    for (int i = 0; i < 4; i++) {
        QDebug deb = qDebug();
        deb << "shake chance" << shakeChance;
        if ((QRandomGenerator::global()->generate() % 65536) >= shakeChance) { //TODO: force fail for debug
            deb << "Shake " + QString::number(i+1) + " failed";
            return false;
        }
        deb << "Shake " + QString::number(i+1) + " success";
        //if (i < 3) {
        //    emit shook();
        //}
    }
    return true;
}
