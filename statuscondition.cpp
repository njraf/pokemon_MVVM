#include "statuscondition.h"

#include <QRandomGenerator>
#include <QDebug>

StatusCondition::StatusCondition(QObject *parent)
    : QObject(parent)
    , burned(false)
    , frozen(false)
    , paralyzed(false)
    , asleep(false)
    , poisoned(false)
    , badlyPoisoned(false)
    , confused(false)
    , infatuated(false)
    , statusCondition(Status::NONE)
    , badPoisonTurn(0)
    , maxSleepTurns(0)
    , numTurnsAsleep(0)
    , maxConfusionTurns(0)
    , numTurnsConfused(0)
{

}

double StatusCondition::damageMultiplier(QSharedPointer<AttackMove> move) const {
    return (burned && move->getCategory() == Category::PHYSICAL) ? 0.5 : 1.0;
}

double StatusCondition::speedMultiplier() const {
    return (paralyzed ? 0.5 : 1.0);
}

bool StatusCondition::canMove() {
    if (frozen) {
        frozen = ((QRandomGenerator::global()->generate() % 5) == 0); // 20% chance to thaw
        return frozen;
    } else if (asleep) {
        if (numTurnsAsleep == maxSleepTurns) {
            numTurnsAsleep = 0;
            maxSleepTurns = 0;
            setAsleep(false);
        }
        return !asleep;
    } else if (paralyzed) { // process paralysis
        return ((QRandomGenerator::global()->generate() % 4) != 0); // 25% chance to not attack
    } else if (infatuated) {
        //qDebug() << name << "is infatuated and cannot attack.";
        return ((QRandomGenerator::global()->generate() % 2) == 0); // 50% chance to not attack
    } else if (confused) {
        if (numTurnsConfused == maxConfusionTurns) {
            numTurnsConfused = 0;
            maxConfusionTurns = 0;
            confused = false;
            qDebug() << "snapped out of confusion";
        } else if ((QRandomGenerator::global()->generate() % 3) == 0) { // hurt by confusion
            emit hurtByConfusion();
            return false;
        }
    }

    return true;
}

void StatusCondition::incrementTurnCounters() {
    numTurnsAsleep++;
    numTurnsConfused++;
    badPoisonTurn++;
}

double StatusCondition::getStatusDamageMultiplier() {
    if (burned) {
        return 0.0625;
    } else if (poisoned) {
        return 0.125;
    } else if (badlyPoisoned) {
        setBadPoisonTurn(getBadPoisonTurn() + 1);
        return static_cast<double>(badPoisonTurn) * 0.0625;
    }
    return 0.0;
}

void StatusCondition::resetStatuses() {
    setConfused(false);
    setInfatuated(false);
}





Status StatusCondition::getStatusCondition() {
    return statusCondition;
}

void StatusCondition::setStatusCondition(Status status_) {
    switch (status_) {
    case Status::ASLEEP:
        setAsleep(true);
        break;
    case Status::PARALYZED:
        setParalyzed(true);
        break;
    case Status::FROZEN:
        setFrozen(true);
        break;
    case Status::POISONED:
        setPoisoned(true);
        break;
    case Status::BADLY_POISONED:
        setBadlyPoisoned(true);
        break;
    case Status::BURNED:
        setBurned(true);
        break;
    default: // Status::NONE
        setAsleep(false);
        setParalyzed(false);
        setFrozen(false);
        setPoisoned(false);
        setBadlyPoisoned(false);
        setBurned(false);
        break;
    }
}

bool StatusCondition::getBurned() const
{
    return burned;
}

void StatusCondition::setBurned(bool newBurned)
{
    if (newBurned &&
            (!burned) &&
            (!frozen) &&
            (!paralyzed) &&
            (!poisoned) &&
            (!badlyPoisoned) &&
            (!asleep))
    {
        burned = newBurned;
        statusCondition = Status::BURNED;
    } else if (!newBurned) {
        burned = false;
        statusCondition = Status::NONE;
    }
}

bool StatusCondition::getFrozen() const
{
    return frozen;
}

void StatusCondition::setFrozen(bool newFrozen)
{
    if (newFrozen &&
            (!burned) &&
            (!frozen) &&
            (!paralyzed) &&
            (!poisoned) &&
            (!badlyPoisoned) &&
            (!asleep))
    {
        frozen = newFrozen;
        statusCondition = Status::FROZEN;
    } else if (!newFrozen) {
        frozen = false;
        statusCondition = Status::NONE;
    }
}

bool StatusCondition::getParalyzed() const
{
    return paralyzed;
}

void StatusCondition::setParalyzed(bool newParalyzed)
{
    if (newParalyzed &&
            (!burned) &&
            (!frozen) &&
            (!paralyzed) &&
            (!poisoned) &&
            (!badlyPoisoned) &&
            (!asleep))
    {
        paralyzed = newParalyzed;
        statusCondition = Status::PARALYZED;
    } else if (!newParalyzed) {
        paralyzed = false;
        statusCondition = Status::NONE;
    }
}

bool StatusCondition::getAsleep() const
{
    return asleep;
}

void StatusCondition::setAsleep(bool newAsleep)
{
    if (newAsleep &&
            (!burned) &&
            (!frozen) &&
            (!paralyzed) &&
            (!poisoned) &&
            (!badlyPoisoned) &&
            (!asleep))
    {
        asleep = newAsleep;
        maxSleepTurns = (QRandomGenerator::global()->generate() % 3) + 1;
        numTurnsAsleep = 0;
        statusCondition = Status::ASLEEP;
    } else if (!newAsleep) {
        asleep = false;
        statusCondition = Status::NONE;
    }
}

bool StatusCondition::getPoisoned() const
{
    return poisoned;
}

void StatusCondition::setPoisoned(bool newPoisoned)
{
    if (newPoisoned &&
            (!burned) &&
            (!frozen) &&
            (!paralyzed) &&
            (!poisoned) &&
            (!badlyPoisoned) &&
            (!asleep))
    {
        poisoned = newPoisoned;
        statusCondition = Status::POISONED;
    } else if (newPoisoned && badlyPoisoned) {
        setBadlyPoisoned(false);
        poisoned = true;
        statusCondition = Status::POISONED;
    } else if (!newPoisoned) {
        poisoned = false;
        statusCondition = Status::NONE;
    }
}

bool StatusCondition::getBadlyPoisoned() const
{
    return badlyPoisoned;
}

void StatusCondition::setBadlyPoisoned(bool newBadlyPoisoned)
{
    if (newBadlyPoisoned &&
            (!burned) &&
            (!frozen) &&
            (!paralyzed) &&
            (!badlyPoisoned) &&
            (!asleep))
    {
        badlyPoisoned = newBadlyPoisoned;
        badPoisonTurn = 0;
        statusCondition = Status::BADLY_POISONED;
    } else if (!newBadlyPoisoned) {
        badlyPoisoned = false;
        statusCondition = Status::NONE;
    }
}

bool StatusCondition::getConfused() const
{
    return confused;
}

void StatusCondition::setConfused(bool newConfused)
{
    if (newConfused) {
        if (!confused) {
            maxConfusionTurns = (QRandomGenerator::global()->generate() % 4) + 2;
            numTurnsConfused = 0;
        }
    }
    confused = newConfused;
}

bool StatusCondition::getInfatuated() const
{
    return infatuated;
}

void StatusCondition::setInfatuated(bool newInfatuated)
{
    infatuated = newInfatuated;
}

int StatusCondition::getBadPoisonTurn() const
{
    return badPoisonTurn;
}

void StatusCondition::setBadPoisonTurn(int newBadPoisonTurn)
{
    badPoisonTurn = newBadPoisonTurn;
    if (badPoisonTurn > 15) {
        badPoisonTurn = 15;
    }
}

int StatusCondition::getMaxSleepTurns() const
{
    return maxSleepTurns;
}

void StatusCondition::setMaxSleepTurns(int newMaxSleepTurns)
{
    maxSleepTurns = newMaxSleepTurns;
}

int StatusCondition::getNumTurnsAsleep() const
{
    return numTurnsAsleep;
}

void StatusCondition::setNumTurnsAsleep(int newNumTurnsAsleep)
{
    numTurnsAsleep = newNumTurnsAsleep;
}

int StatusCondition::getMaxConfusionTurns() const
{
    return maxConfusionTurns;
}

void StatusCondition::setMaxConfusionTurns(int newMaxConfusionTurns)
{
    maxConfusionTurns = newMaxConfusionTurns;
}

int StatusCondition::getNumTurnsConfused() const
{
    return numTurnsConfused;
}

void StatusCondition::setNumTurnsConfused(int newNumTurnsConfused)
{
    numTurnsConfused = newNumTurnsConfused;
}

QString StatusCondition::toName() {
    switch (statusCondition) {
    case Status::NONE:
        return "";
    case Status::BURNED:
        return "BRN";
    case Status::FROZEN:
        return "FRZ";
    case Status::PARALYZED:
        return "PAR";
    case Status::POISONED:
        return "PSN";
    case Status::BADLY_POISONED:
        return "PSN";
    case Status::ASLEEP:
        return "SLP";
    default:
        return "";
    }
}

QString StatusCondition::toColor() {
    switch (statusCondition) {
    case Status::NONE:
        return "lightgray";
    case Status::BURNED:
        return "red";
    case Status::FROZEN:
        return "cyan";
    case Status::PARALYZED:
        return "yellow";
    case Status::POISONED:
        return "purple";
    case Status::BADLY_POISONED:
        return "purple";
    case Status::ASLEEP:
        return "aquamarine";
    default:
        return "white";
    }
}

