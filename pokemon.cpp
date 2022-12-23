#include "pokemon.h"

Pokemon::Pokemon(QString name_, int attackStat_, int spAttackStat_, int defenceStat_, int spDefenceStat_, int healthStat_, int speedStat_)
    : name(name_)
    , attackStat(attackStat_)
    , spAttackStat(spAttackStat_)
    , defenceStat(defenceStat_)
    , spDefenceStat(spDefenceStat_)
    , healthStat(healthStat_)
    , speedStat(speedStat_)
{

}

QString Pokemon::getName() {
    return name;
}





