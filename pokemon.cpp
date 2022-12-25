#include "pokemon.h"

#include <QDebug>
#include <QRandomGenerator>

Pokemon::Pokemon(QString name_, int attackStat_, int spAttackStat_, int defenseStat_, int spDefenseStat_, int healthStat_, int speedStat_, int maxHealthStat_, int level_, QVector<QSharedPointer<AttackMove>> attackList_, Type type1_, Type type2_)
    : name(name_)
    , attackStat(attackStat_)
    , spAttackStat(spAttackStat_)
    , defenseStat(defenseStat_)
    , spDefenseStat(spDefenseStat_)
    , healthStat(healthStat_)
    , speedStat(speedStat_)
    , maxHealthStat(maxHealthStat_)
    , level(level_)
    , attackList(attackList_)
    , type1(type1_)
    , type2(type2_)
{

}

void Pokemon::attack(QSharedPointer<Pokemon> opponent, QSharedPointer<AttackMove> attackMove) {
    double attackPower = (double)attackMove->getPower();
    double stab = ((attackMove->getType() == type1) || (attackMove->getType() == type2)) ? 1.5 : 1.0;
    double weakResist = TypeUtilities::calcEffectiveness(attackMove->getType(), opponent->getType1());
    weakResist *= TypeUtilities::calcEffectiveness(attackMove->getType(), opponent->getType2());
    double randomNumber = (double)((QRandomGenerator::global()->generate() % 16) + 85); // get a random number (85 - 100)

    //TODO: include stat change stages from Swords Dance and others

    double damage = ((((2.0 * ((double)level) / 5.0 + 2.0) * attackStat * attackPower / defenseStat) / 50.0) + 2.0) * stab * weakResist * randomNumber / 100.0;
    int dmg = ((damage - qFloor(damage)) < 0.5) ? qFloor(damage) : qCeil(damage); // round damage to the nearest whole number

    opponent->setHealthStat(opponent->getHealthStat() - dmg);
    qDebug() << name << "attacked" << opponent->getName() << "with" << attackMove->getName() << "and dealt" << dmg << "damage.";
    emit attacked();
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
    healthStat = (newHealthStat < 0 ? 0 : newHealthStat);
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

QVector<QSharedPointer<AttackMove> > Pokemon::getAttackList() const {
    return attackList;
}





