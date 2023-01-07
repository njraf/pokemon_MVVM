#include "pokeballitem.h"

#include <QtMath>
#include <QRandomGenerator>

PokeballItem::PokeballItem(QString name_, double catchRate_, QObject *parent)
    : QObject(parent)
    , name(name_)
    , catchRate(catchRate_)
{

}

bool PokeballItem::throwAtPokemon(QSharedPointer<Pokemon> pokemon) {
    double catchChance = qFloor((3 * pokemon->getMaxHealthStat() - 2 * pokemon->getHealthStat()) * 4096 + 0.5); // TODO: multiply floored value by species catch rate
    catchChance *= catchRate; // pokeball catch rate
    catchChance = qFloor(((36 - 2 * pokemon->getLevel()) * catchChance) / 10); // make it easier to cathc pokemon level 13 and under
    catchChance *= pokemon->getStatusCondition()->catchMultiplier(); // status condition multiplier
    for (int i = 0; i < 4; i++) {
        double shakeChance = qPow(65536 / ((255 * 4096) / catchChance), 0.1875);
        if (QRandomGenerator::global()->generate() >= shakeChance) {
            return false;
        }

        //if (i < 3) {
        //    emit shook();
        //}
    }
    return true;
}
