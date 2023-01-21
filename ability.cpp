#include "ability.h"

Ability::Ability(BattleStage stage_, Target target_, std::function<void(QSharedPointer<Pokemon>)> ability_, QObject *parent)
    : QObject(parent)
    , battleStage(stage_)
    , target(target_)
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

void Ability::useAbility(QSharedPointer<Pokemon> abilityUser, QSharedPointer<Pokemon> opponentPokemon, BattleStage stage_) {
    if (stage_ == battleStage) {
        QSharedPointer<Pokemon> targetPokemon = (target == Target::SELF) ? abilityUser : opponentPokemon;
        ability(targetPokemon);
    }
}

BattleStage Ability::getBattleStage() {
    return battleStage;
}
