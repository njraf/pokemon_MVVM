#include "abilityfactory.h"

#include <QDebug>

AbilityFactory::AbilityFactory(QObject *parent) : QObject(parent)
{

}

std::function<void(QSharedPointer<Pokemon>, QSharedPointer<Pokemon>)> AbilityFactory::getAbility(int id) {
    std::function<void(QSharedPointer<Pokemon>, QSharedPointer<Pokemon>)> ability;
    switch (id) {
    case 0: // intimidate
        ability = [](QSharedPointer<Pokemon> playerPokemon, QSharedPointer<Pokemon> opponentPokemon) {
            opponentPokemon->setAttackStatStage(opponentPokemon->getAttackStatStage() - 1);
        };
        break;
    default:
        ability = [](QSharedPointer<Pokemon> playerPokemon, QSharedPointer<Pokemon> opponentPokemon) {};
        break;
    }

    return ability;
}
