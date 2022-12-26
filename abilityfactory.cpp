#include "abilityfactory.h"

#include <QDebug>

AbilityFactory::AbilityFactory(QObject *parent) : QObject(parent)
{

}

std::function<void(QSharedPointer<Pokemon>, QSharedPointer<Pokemon>)> AbilityFactory::getAbility(int id) {
    std::function<void(QSharedPointer<Pokemon>, QSharedPointer<Pokemon>)> ability;
    switch (id) {
    case 0: // intimidate
        ability = [](QSharedPointer<Pokemon> attackingPokemon, QSharedPointer<Pokemon> opponentPokemon) {
            opponentPokemon->setAttackStatStage(opponentPokemon->getAttackStatStage() - 1);
        };
        break;
    case 1: // aftermath
        ability = [](QSharedPointer<Pokemon> attackingPokemon, QSharedPointer<Pokemon> opponentPokemon) {
            opponentPokemon->setHealthStat(opponentPokemon->getHealthStat() - (0.25 * opponentPokemon->getMaxHealthStat())); //TODO: only activate when defeated by a physical move
            qDebug() << "Aftermath";
        };
        break;
    default:
        ability = [](QSharedPointer<Pokemon> attackingPokemon, QSharedPointer<Pokemon> opponentPokemon) {};
        break;
    }

    return ability;
}
