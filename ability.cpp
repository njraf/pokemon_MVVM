#include "ability.h"

Ability::Ability(BattleStage stage_, std::function<void(QSharedPointer<Pokemon>, QSharedPointer<Pokemon>)> ability_, QObject *parent)
    : QObject(parent)
    , battleStage(stage_)
    , ability(ability_)
{

}

Ability::Ability(const Ability &o) {
    battleStage = o.battleStage;
    ability = o.ability;
}

Ability& Ability::operator=(Ability o) {
    battleStage = o.battleStage;
    ability = o.ability;
    return *this;
}

void Ability::useAbility(QSharedPointer<Pokemon> playerPokemon, QSharedPointer<Pokemon> opponentPokemon) {
    ability(playerPokemon, opponentPokemon);
}

BattleStage Ability::getBattleStage() {
    return battleStage;
}
