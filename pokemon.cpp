#include "pokemon.h"

Pokemon::Pokemon(QString name_, int attackStat_, int spAttackStat_, int defenseStat_, int spDefenseStat_, int healthStat_, int speedStat_, int maxHealthStat_, int level_, Type type1_, Type type2_)
    : name(name_)
    , attackStat(attackStat_)
    , spAttackStat(spAttackStat_)
    , defenseStat(defenseStat_)
    , spDefenseStat(spDefenseStat_)
    , healthStat(healthStat_)
    , speedStat(speedStat_)
    , maxHealthStat(maxHealthStat_)
    , level(level_)
    , type1(type1_)
    , type2(type2_)
{

}

void Pokemon::attack(QSharedPointer<Pokemon> opponent) {
    //TODO: get attack power of the selected move
    int attackPower = 0; //TODO: get attack power from the selected move
    double stab = 1.0; //TODO: calculate STAB
    double weakResist = 1.0; //TODO: calculate based on selected move type and opponent's type(s)
    int randomNumber = 1; //TODO: get a random number (85 - 100)

    //TODO: include stat change stages from Swords Dance and others

    int damage = ((((2 * level / 5 + 2) * attackStat * attackPower / defenseStat) / 50) + 2) * stab * weakResist * randomNumber / 100;
}

QString Pokemon::getName() const {
    return name;
}

void Pokemon::setName(const QString &newName) {
    name = newName;
}

int Pokemon::getAttackStat() const {
    return attackStat;
}

void Pokemon::setAttackStat(int newAttackStat) {
    attackStat = newAttackStat;
}

int Pokemon::getSpAttackStat() const {
    return spAttackStat;
}

void Pokemon::setSpAttackStat(int newSpAttackStat) {
    spAttackStat = newSpAttackStat;
}

int Pokemon::getDefenseStat() const {
    return defenseStat;
}

void Pokemon::setDefenseStat(int newDefenseStat) {
    defenseStat = newDefenseStat;
}

int Pokemon::getSpDefenseStat() const {
    return spDefenseStat;
}

void Pokemon::setSpDefenseStat(int newSpDefenseStat) {
    spDefenseStat = newSpDefenseStat;
}

int Pokemon::getHealthStat() const {
    return healthStat;
}

void Pokemon::setHealthStat(int newHealthStat) {
    healthStat = newHealthStat;
}

int Pokemon::getSpeedStat() const {
    return speedStat;
}

void Pokemon::setSpeedStat(int newSpeedStat) {
    speedStat = newSpeedStat;
}

int Pokemon::getMaxHealthStat() const {
    return maxHealthStat;
}

void Pokemon::setMaxHealthStat(int newMaxHealthStat) {
    maxHealthStat = newMaxHealthStat;
}

int Pokemon::getLevel() const {
    return level;
}

void Pokemon::setLevel(int newLevel) {
    level = newLevel;
}

Type Pokemon::getType1() const {
    return type1;
}

Type Pokemon::getType2() const {
    return type2;
}





