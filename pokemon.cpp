#include "pokemon.h"
#include "attackeffectfactory.h"

#include <QDebug>
#include <QRandomGenerator>

Pokemon::Pokemon(int id_, int natDexNumber_, QString name_, QString owner_, int boxNumber_, Nature nature_, int baseMaxHealthStat_, int baseAttackStat_, int baseDefenseStat_,
                 int baseSpAttackStat_, int baseSpDefenseStat_, int baseSpeedStat_, int currentHealthStat_, int level_,
                 QVector<QSharedPointer<AttackMove>> attackList_, Type type1_, Type type2_)
    : id(id_)
    , baseAttackStat(baseAttackStat_)
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
    , attackStatStage(0)
    , spAttackStatStage(0)
    , defenseStatStage(0)
    , spDefenseStatStage(0)
    , speedStatStage(0)
    , accuracyStatStage(0)
    , evasionStatStage(0)
    , natDexNumber(natDexNumber_)
    , name(name_)
    , owner(owner_)
    , nature(nature_)
    , currentHealthStat(currentHealthStat_)
    , level(level_)
    , attackList(attackList_)
    , type1(type1_)
    , type2(type2_)
    , boxNumber(boxNumber_)
    , status(QSharedPointer<StatusCondition>::create())
{
    // calculate IVs
    attackStatIV = QRandomGenerator::global()->generate() % 32;
    spAttackStatIV = QRandomGenerator::global()->generate() % 32;
    defenseStatIV = QRandomGenerator::global()->generate() % 32;
    spDefenseStatIV = QRandomGenerator::global()->generate() % 32;
    speedStatIV = QRandomGenerator::global()->generate() % 32;
    maxHealthStatIV = QRandomGenerator::global()->generate() % 32;

    if (currentHealthStat > getMaxHealthStat()) {
        currentHealthStat = getMaxHealthStat();
    }

    connect(status.data(), &StatusCondition::hurtByConfusion, this, [=] {
        auto self = QSharedPointer<Pokemon>(this, [](Pokemon *p){});
        QSharedPointer<AttackMove> confusionStatus = QSharedPointer<AttackMove>::create(0, "Confusion Status", 40, 100, 99, 99, Type::NONE, Category::PHYSICAL, AttackEffectFactory::getEffectByID(-1));
        int dmg = calculateDamage(self, confusionStatus);
        setHealthStat(currentHealthStat - dmg);
        qDebug() << name << "hit itself in confusion";
    });
}

void Pokemon::attack(QSharedPointer<Pokemon> opponent, QSharedPointer<AttackMove> attackMove) {

    if (attackMove->getPp() <= 0) {
        qDebug() << "This move has no PP left";
        return;
    }

    if (!status->canMove()) {
        emit attacked();
        return;
    }

    attackMove->setPp(attackMove->getPp() - 1);

    // calculate accuracy
    int combinedStages = accuracyStatStage - opponent->getEvasionStatStage();
    if (combinedStages > 6) {
        combinedStages = 6;
    } else if (combinedStages < -6) {
        combinedStages = -6;
    }

    qreal dAccuracy = (double)attackMove->getAccuracy() * getAccuracyStageMultiplier(combinedStages);
    quint32 iAccuracy = ((dAccuracy - qFloor(dAccuracy)) < 0.5) ? qFloor(dAccuracy) : qCeil(dAccuracy);
    if ((attackMove->getAccuracy() > 0) && ((QRandomGenerator::global()->generate() % 101) > iAccuracy)) {
        qDebug() << "The attack missed";
        emit attacked();
        return;
    }


    // calculate damage
    if (attackMove->getPower() > 0) {
        int dmg = calculateDamage(opponent, attackMove);
        opponent->setHealthStat(opponent->getHealthStat() - dmg);
        qDebug() << name << "attacked" << opponent->getName() << "with" << attackMove->getName() << "and dealt" << dmg << "damage.";

        if ((opponent->getStatusCondition()->getFrozen()) && (attackMove->getType() == Type::FIRE) && (attackMove->getPower() > 0)) {
            opponent->getStatusCondition()->setStatusCondition(Status::NONE);
        }
    }

    auto self = QSharedPointer<Pokemon>(this, [](Pokemon *p){});
    attackMove->useEffect(self, opponent);

    emit attacked();
}


int Pokemon::calculateDamage(QSharedPointer<Pokemon> opponent, QSharedPointer<AttackMove> attackMove) {
    double attackPower = (double)attackMove->getPower();
    double stab = ((attackMove->getType() == type1) || (attackMove->getType() == type2)) ? 1.5 : 1.0;
    double weakResist = TypeUtilities::calcEffectiveness(attackMove->getType(), opponent->getType1());
    weakResist *= TypeUtilities::calcEffectiveness(attackMove->getType(), opponent->getType2());
    double randomNumber = (double)((QRandomGenerator::global()->generate() % 16) + 85); // get a random number (85 - 100)

    double attack = 1.0;
    double defense = 1.0;
    if (attackMove->getCategory() == Category::PHYSICAL) {
        attack = getAttackStat();
        defense = opponent->getDefenseStat();
    } else if (attackMove->getCategory() == Category::SPECIAL) {
        attack = getSpAttackStat();
        defense = opponent->getSpDefenseStat();
    }

    double damage = ((((2.0 * ((double)level) / 5.0 + 2.0) * attack * attackPower / defense) / 50.0) + 2.0) * stab * weakResist * randomNumber * status->damageMultiplier(attackMove) / 100;
    int dmg = ((damage - qFloor(damage)) < 0.5) ? qFloor(damage) : qCeil(damage); // round damage to the nearest whole number
    return dmg;
}

int Pokemon::getID() {
    return id;
}

int Pokemon::getDexNumber() {
    return natDexNumber;
}

QString Pokemon::getName() const {
    return name;
}

void Pokemon::setName(const QString &newName) {
    name = newName;
}

QString Pokemon::getOwner() {
    return owner;
}

void Pokemon::setOwner(QString newOwner) {
    owner = newOwner;
}

int Pokemon::getAttackStat() const {
    return ((((double)attackStatIV + 2.0 * (double)baseAttackStat + ((double)attackStatEV / 4.0)) * (double)level / 100.0) + 5.0) *
            NatureUtilities::calcMultiplier(nature, NatureUtilities::Stat::ATTACK) * getStatStageMultiplier(attackStatStage);
}

int Pokemon::getSpAttackStat() const {
    return ((((double)spAttackStatIV + 2.0 * (double)baseSpAttackStat + ((double)spAttackStatEV / 4.0)) * (double)level / 100.0) + 5.0) *
            NatureUtilities::calcMultiplier(nature, NatureUtilities::Stat::SP_ATTACK) * getStatStageMultiplier(spAttackStatStage);
}

int Pokemon::getDefenseStat() const {
    return ((((double)defenseStatIV + 2.0 * (double)baseDefenseStat + ((double)defenseStatEV / 4.0)) * (double)level / 100.0) + 5.0) *
            NatureUtilities::calcMultiplier(nature, NatureUtilities::Stat::DEFENSE) * getStatStageMultiplier(defenseStatStage);
}

int Pokemon::getSpDefenseStat() const {
    return ((((double)spDefenseStatIV + 2.0 * (double)baseSpDefenseStat + ((double)spDefenseStatEV / 4.0)) * (double)level / 100.0) + 5.0) *
            NatureUtilities::calcMultiplier(nature, NatureUtilities::Stat::SP_DEFENSE) * getStatStageMultiplier(spDefenseStatStage);
}

int Pokemon::getHealthStat() const {
    return currentHealthStat;
}

void Pokemon::setHealthStat(int newHealthStat) {
    bool damaged = (newHealthStat < currentHealthStat);
    currentHealthStat = (newHealthStat < 0 ? 0 : newHealthStat);
    currentHealthStat = (currentHealthStat > getMaxHealthStat() ? getMaxHealthStat() : currentHealthStat);
    if (damaged) {
        emit tookDamage();
    }
}

int Pokemon::getSpeedStat() const {
    return static_cast<int>(((((double)speedStatIV + 2.0 * (double)baseSpeedStat + ((double)speedStatEV / 4.0)) * (double)level / 100.0) + 5.0) *
            NatureUtilities::calcMultiplier(nature, NatureUtilities::Stat::SPEED) * getStatStageMultiplier(speedStatStage) *
            status->speedMultiplier());
}

int Pokemon::getMaxHealthStat() const {
    return (((double)maxHealthStatIV + 2.0 * (double)baseMaxHealthStat + ((double)maxHealthStatEV / 4.0)) * (double)level / 100.0) + 10.0 + (double)level;
}

int Pokemon::getAttackStatIV() {
    return attackStatIV;
}

void Pokemon::setAttackStatIV(int newStat) {
    attackStatIV = newStat;
}

int Pokemon::getSpAttackStatIV() {
    return spAttackStatIV;
}

void Pokemon::setSpAttackStatIV(int newStat) {
    spAttackStatIV = newStat;
}

int Pokemon::getDefenseStatIV() {
    return defenseStatIV;
}

void Pokemon::setDefenseStatIV(int newStat) {
    defenseStatIV = newStat;
}

int Pokemon::getSpDefenseStatIV() {
    return spDefenseStatIV;
}

void Pokemon::setSpDefenseStatIV(int newStat) {
    spDefenseStatIV = newStat;
}

int Pokemon::getSpeedStatIV() {
    return speedStatIV;
}

void Pokemon::setSpeedStatIV(int newStat) {
    speedStatIV = newStat;
}

int Pokemon::getMaxHealthStatIV() {
    return maxHealthStatIV;
}

void Pokemon::setMaxHealthStatIV(int newStat) {
    maxHealthStatIV = newStat;
}

int Pokemon::getAttackStatEV() {
    return attackStatEV;
}

void Pokemon::setAttackStatEV(int newStat) {
    attackStatEV = newStat;
}

int Pokemon::getSpAttackStatEV() {
    return spAttackStatEV;
}

void Pokemon::setSpAttackStatEV(int newStat) {
    spAttackStatEV = newStat;
}

int Pokemon::getDefenseStatEV() {
    return defenseStatEV;
}

void Pokemon::setDefenseStatEV(int newStat) {
    defenseStatEV = newStat;
}

int Pokemon::getSpDefenseStatEV() {
    return spDefenseStatEV;
}

void Pokemon::setSpDefenseStatEV(int newStat) {
    spDefenseStatEV = newStat;
}

int Pokemon::getSpeedStatEV() {
    return speedStatEV;
}

void Pokemon::setSpeedStatEV(int newStat) {
    speedStatEV = newStat;
}

int Pokemon::getMaxHealthStatEV() {
    return maxHealthStatEV;
}

void Pokemon::setMaxHealthStatEV(int newStat) {
    maxHealthStatEV = newStat;
}

int Pokemon::getAttackStatStage() const {
    return attackStatStage;
}

void Pokemon::setAttackStatStage(int stage) {
    if (stage > 6) {
        attackStatStage = 6;
    } else if (stage < -6) {
        attackStatStage = -6;
    } else {
        attackStatStage = stage;
    }
}

int Pokemon::getSpAttackStatStage() const {
    return spAttackStatStage;
}

void Pokemon::setSpAttackStatStage(int stage) {
    if (stage > 6) {
        spAttackStatStage = 6;
    } else if (stage < -6) {
        spAttackStatStage = -6;
    } else {
        spAttackStatStage = stage;
    }
}

int Pokemon::getDefenseStatStage() const {
    return defenseStatStage;
}

void Pokemon::setDefenseStatStage(int stage) {
    if (stage > 6) {
        defenseStatStage = 6;
    } else if (stage < -6) {
        defenseStatStage = -6;
    } else {
        defenseStatStage = stage;
    }
}

int Pokemon::getSpDefenseStatStage() const {
    return spDefenseStatStage;
}

void Pokemon::setSpDefenseStatStage(int stage) {
    if (stage > 6) {
        spDefenseStatStage = 6;
    } else if (stage < -6) {
        spDefenseStatStage = -6;
    } else {
        spDefenseStatStage = stage;
    }
}

int Pokemon::getSpeedStatStage() const {
    return speedStatStage;
}

void Pokemon::setSpeedStatStage(int stage) {
    if (stage > 6) {
        speedStatStage = 6;
    } else if (stage < -6) {
        speedStatStage = -6;
    } else {
        speedStatStage = stage;
    }
}

int Pokemon::getAccuracyStatStage() const {
    return accuracyStatStage;
}

void Pokemon::setAccuracyStatStage(int stage) {
    if (stage > 6) {
        accuracyStatStage = 6;
    } else if (stage < -6) {
        accuracyStatStage = -6;
    } else {
        accuracyStatStage = stage;
    }
}

int Pokemon::getEvasionStatStage() const {
    return evasionStatStage;
}

void Pokemon::resetAllStages() {
    attackStatStage = 0;
    spAttackStatStage = 0;
    defenseStatStage = 0;
    spDefenseStatStage = 0;
    speedStatStage = 0;
    accuracyStatStage = 0;
    evasionStatStage = 0;
}

void Pokemon::setEvasionStatStage(int stage) {
    if (stage > 6) {
        evasionStatStage = 6;
    } else if (stage < -6) {
        evasionStatStage = -6;
    } else {
        evasionStatStage = stage;
    }
}

double Pokemon::getStatStageMultiplier(int stage) const {
    switch (stage) {
    case -6:
        return 0.25;
    case -5:
        return 0.285;
    case -4:
        return 0.33;
    case -3:
        return 0.4;
    case -2:
        return 0.5;
    case -1:
        return 0.66;
    case 0:
        return 1.0;
    case 1:
        return 1.5;
    case 2:
        return 2.0;
    case 3:
        return 2.5;
    case 4:
        return 3.0;
    case 5:
        return 3.5;
    case 6:
        return 4.0;
    default:
        return 1.0;
    }
}

double Pokemon::getAccuracyStageMultiplier(int stage) const {
    switch (stage) {
    case -6:
        return 0.33;
    case -5:
        return 0.375;
    case -4:
        return 0.428;
    case -3:
        return 0.5;
    case -2:
        return 0.6;
    case -1:
        return 0.75;
    case 0:
        return 1.0;
    case 1:
        return 1.33;
    case 2:
        return 1.66;
    case 3:
        return 2.0;
    case 4:
        return 2.33;
    case 5:
        return 2.66;
    case 6:
        return 3.0;
    default:
        return 1.0;
    }
}

double Pokemon::getEvasionStageMultiplier(int stage) const {
    switch (stage) {
    case 6:
        return 0.33;
    case 5:
        return 0.375;
    case 4:
        return 0.428;
    case 3:
        return 0.5;
    case 2:
        return 0.6;
    case 1:
        return 0.75;
    case 0:
        return 1.0;
    case -1:
        return 1.33;
    case -2:
        return 1.66;
    case -3:
        return 2.0;
    case -4:
        return 2.33;
    case -5:
        return 2.66;
    case -6:
        return 3.0;
    default:
        return 1.0;
    }
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

Nature Pokemon::getNature() const {
    return nature;
}

int Pokemon::getBoxNumber() const {
    return boxNumber;
}

void Pokemon::setBoxNumber(int newBoxNumber) {
    boxNumber = newBoxNumber;
}

QVector<QSharedPointer<AttackMove> > Pokemon::getAttackList() const {
    return attackList;
}

void Pokemon::setAttackList(QVector<QSharedPointer<AttackMove> > newAttackList) {
    attackList = newAttackList;
}

QSharedPointer<StatusCondition> Pokemon::getStatusCondition() {
    return status;
}

void Pokemon::setStatusCondition(Status status_) {
    if (status->getStatusCondition() == Status::NONE) {
        if (((status_ == Status::FROZEN) && (type1 != Type::ICE) && (type2 != Type::ICE)) || // do not freeze ice types
                ((status_ == Status::BURNED) && (type1 != Type::FIRE) && (type2 != Type::FIRE)) || // do not burn fire types
                ((status_ == Status::PARALYZED) && (type1 != Type::ELECTRIC) && (type2 != Type::ELECTRIC)) || // do not paralyze electric types
                (((status_ == Status::POISONED) || (status_ == Status::BADLY_POISONED)) && (type1 != Type::POISON) && (type2 != Type::POISON) && (type1 != Type::STEEL) && (type2 != Type::STEEL)) // do not poison steel or poison types
                )
        {
            status->setStatusCondition(status_);
        } else if ((status->getStatusCondition() == Status::BADLY_POISONED) && (status_ == Status::POISONED)) {
            status->setStatusCondition(Status::POISONED); // force downgrade bad poison to poison
        } else if (status_ == Status::ASLEEP) {
            status->setStatusCondition(status_);
        }
        emit statusConditionSet(status_);
    }
}
