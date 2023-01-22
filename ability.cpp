#include "ability.h"

QMap<QString, BattleStage> Ability::strToBattleStage = {
    {"SUMMON", BattleStage::SUMMON},
    {"BEFORE_ATTACK", BattleStage::BEFORE_ATTACK},
    {"AFTER_ATTACK", BattleStage::AFTER_ATTACK},
    {"BEFORE_OPPONENT_ATTACK", BattleStage::BEFORE_OPPONENT_ATTACK},
    {"AFTER_OPPONENT_ATTACK", BattleStage::AFTER_OPPONENT_ATTACK},
    {"ATTACK_HITS", BattleStage::ATTACK_HITS},
    {"OPPONENT_HITS", BattleStage::OPPONENT_HITS},
    {"START_TURN", BattleStage::START_TURN},
    {"END_TURN", BattleStage::END_TURN},
    {"FAINT", BattleStage::FAINT}
};

QMap<QString, Ability::Target> Ability::strToTarget {
    {"SELF", Ability::Target::SELF},
    {"ALLY", Ability::Target::ALLY},
    {"OPPONENT", Ability::Target::OPPONENT}
};

Ability::Ability(int id_, QString name_, BattleStage stage_, Target target_, std::function<void(QSharedPointer<Pokemon>)> ability_, QObject *parent)
    : QObject(parent)
    , id(id_)
    , name(name_)
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
    id = o.id;
    name = o.name;
    battleStage = o.battleStage;
    target = o.target;
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
