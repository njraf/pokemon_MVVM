#include "pokemon.h"

#include <QDebug>
#include <QRandomGenerator>

Pokemon::Pokemon(QString name_, QString owner_, Nature nature_, int baseMaxHealthStat_, int baseAttackStat_, int baseDefenseStat_, int baseSpAttackStat_, int baseSpDefenseStat_, int baseSpeedStat_, int currentHealthStat_, int level_, QVector<QSharedPointer<AttackMove>> attackList_, Type type1_, Type type2_)
    : baseAttackStat(baseAttackStat_)
    , baseSpAttackStat(baseSpAttackStat_)
    , baseDefenseStat(baseDefenseStat_)
    , baseSpDefenseStat(baseSpDefenseStat_)
    , baseSpeedStat(baseSpeedStat_)
    , baseMaxHealthStat(baseMaxHealthStat_)
    , attackStatIV(0)
    , spAttackStatIV(0)
    , defenseStatIV(0)
    , spDefenseStatIV(0)
    , speedStatIV(0)
    , maxHealthStatIV(0)
    , attackStatEV(0)
    , spAttackStatEV(0)
    , defenseStatEV(0)
    , spDefenseStatEV(0)
    , speedStatEV(0)
    , maxHealthStatEV(0)
    , name(name_)
    , owner(owner_)
    , nature(nature_)
    , currentHealthStat(currentHealthStat_)
    , level(level_)
    , attackList(attackList_)
    , type1(type1_)
    , type2(type2_)
{
    // calculate IVs
    if (owner.isEmpty()) {
        attackStatIV = QRandomGenerator::global()->generate() % 32;
        spAttackStatIV = QRandomGenerator::global()->generate() % 32;
        defenseStatIV = QRandomGenerator::global()->generate() % 32;
        spDefenseStatIV = QRandomGenerator::global()->generate() % 32;
        speedStatIV = QRandomGenerator::global()->generate() % 32;
        maxHealthStatIV = QRandomGenerator::global()->generate() % 32;
    }

    if (currentHealthStat > getMaxHealthStat()) {
        currentHealthStat = getMaxHealthStat();
    }
}

void Pokemon::attack(QSharedPointer<Pokemon> opponent, QSharedPointer<AttackMove> attackMove) {
    double attackPower = (double)attackMove->getPower();
    double stab = ((attackMove->getType() == type1) || (attackMove->getType() == type2)) ? 1.5 : 1.0;
    double weakResist = TypeUtilities::calcEffectiveness(attackMove->getType(), opponent->getType1());
    weakResist *= TypeUtilities::calcEffectiveness(attackMove->getType(), opponent->getType2());
    double randomNumber = (double)((QRandomGenerator::global()->generate() % 16) + 85); // get a random number (85 - 100)

    //TODO: include stat change stages from Swords Dance and others

    double attack = 0.0;
    double defense = 0.0;
    if (attackMove->getCategory() == Category::PHYSICAL) {
        attack = getAttackStat();
        defense = opponent->getDefenseStat();
    } else if (attackMove->getCategory() == Category::SPECIAL) {
        attack = getSpAttackStat();
        defense = opponent->getSpDefenseStat();
    }

    double damage = ((((2.0 * ((double)level) / 5.0 + 2.0) * attack * attackPower / defense) / 50.0) + 2.0) * stab * weakResist * randomNumber / 100.0;
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
    return (((attackStatIV + 2 * baseAttackStat + (attackStatEV / 4)) * level / 100) + 5) * NatureUtilities::calcMultiplier(nature, NatureUtilities::Stat::ATTACK);
}

int Pokemon::getSpAttackStat() const {
    return (((spAttackStatIV + 2 * baseSpAttackStat + (spAttackStatEV / 4)) * level / 100) + 5) * NatureUtilities::calcMultiplier(nature, NatureUtilities::Stat::SP_ATTACK);
}

int Pokemon::getDefenseStat() const {
    return (((defenseStatIV + 2 * baseDefenseStat + (defenseStatEV / 4)) * level / 100) + 5) * NatureUtilities::calcMultiplier(nature, NatureUtilities::Stat::DEFENSE);
}

int Pokemon::getSpDefenseStat() const {
    return (((spDefenseStatIV + 2 * baseSpDefenseStat + (spDefenseStatEV / 4)) * level / 100) + 5) * NatureUtilities::calcMultiplier(nature, NatureUtilities::Stat::SP_DEFENSE);;
}

int Pokemon::getHealthStat() const {
    return currentHealthStat;
}

void Pokemon::setHealthStat(int newHealthStat) {
    currentHealthStat = (newHealthStat < 0 ? 0 : newHealthStat);
}

int Pokemon::getSpeedStat() const {
    return (((speedStatIV + 2 * baseSpeedStat + (speedStatEV / 4)) * level / 100) + 5) * NatureUtilities::calcMultiplier(nature, NatureUtilities::Stat::SPEED);
}

int Pokemon::getMaxHealthStat() const {
    return ((maxHealthStatIV + 2 * baseMaxHealthStat + (maxHealthStatEV / 4)) * level / 100) + 10 + level;
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





