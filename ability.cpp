#include "ability.h"

QMap<QString, BattleStage> Ability::strToBattleStage = {
    {"SWITCH_IN", BattleStage::SWITCH_IN},
    {"SWITCH_OUT", BattleStage::SWITCH_OUT},
    {"BEFORE_ATTACK", BattleStage::BEFORE_ATTACK},
    {"AFTER_ATTACK", BattleStage::AFTER_ATTACK},
    {"BEFORE_OPPONENT_ATTACK", BattleStage::BEFORE_OPPONENT_ATTACK},
    {"AFTER_OPPONENT_ATTACK", BattleStage::AFTER_OPPONENT_ATTACK},
    {"ATTACK_HITS", BattleStage::ATTACK_HITS},
    {"OPPONENT_HITS", BattleStage::OPPONENT_HITS},
    {"START_TURN", BattleStage::START_TURN},
    {"END_TURN", BattleStage::END_TURN},
    {"FAINT", BattleStage::FAINT},
    {"OPPONENT_FAINT", BattleStage::OPPONENT_FAINT},
    {"STATUS_APPLIED", BattleStage::STATUS_APPLIED},
    {"STATUS_REMOVED", BattleStage::STATUS_REMOVED},
    {"FLINCH", BattleStage::FLINCH}
};

QMap<QString, Ability::Target> Ability::strToTarget {
    {"SELF", Ability::Target::SELF},
    {"ALLY", Ability::Target::ALLY},
    {"OPPONENT", Ability::Target::OPPONENT}
};

Ability::Ability(int id_, QString name_, QVector<BattleStage> stages_, Target target_, Category attackCategory_, Type attackType_, std::function<void(QSharedPointer<Pokemon>)> ability_, QObject *parent)
    : QObject(parent)
    , id(id_)
    , name(name_)
    , battleStages(stages_)
    , target(target_)
    , attackCategory(attackCategory_)
    , attackType(attackType_)
    , ability(ability_)
{

}

Ability::Ability(const Ability &o) {
    id = o.id;
    name = o.name;
    battleStages = o.battleStages;
    target = o.target;
    attackCategory = o.attackCategory;
    attackType = o.attackType;
    ability = o.ability;
}

Ability& Ability::operator=(Ability o) {
    id = o.id;
    name = o.name;
    battleStages = o.battleStages;
    target = o.target;
    attackCategory = o.attackCategory;
    attackType = o.attackType;
    ability = o.ability;
    return *this;
}

void Ability::useAbility(QSharedPointer<Pokemon> abilityUser, QSharedPointer<Pokemon> opponentPokemon, BattleStage stage, Category attackCategory_, Type attackType_) {
    if (battleStages.contains(stage) &&
            ((attackType_ == attackType) || (attackType == Type::NONE)) &&
            ((attackCategory_ == attackCategory) || (attackCategory == Category::NONE))
            )
    {
        QSharedPointer<Pokemon> targetPokemon = (target == Target::SELF) ? abilityUser : opponentPokemon;
        ability(targetPokemon);
    }
}

QVector<BattleStage> Ability::getBattleStages() {
    return battleStages;
}
