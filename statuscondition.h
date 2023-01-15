#ifndef STATUSCONDITION_H
#define STATUSCONDITION_H

#include <QObject>
#include <QSharedPointer>

#include "attackmove.h"

enum class Status : int {
    NONE, BURNED, FROZEN, PARALYZED, POISONED, BADLY_POISONED, ASLEEP
};

class StatusCondition : public QObject
{
    Q_OBJECT
public:
    explicit StatusCondition(QObject *parent = nullptr);
    ~StatusCondition() = default;

    double damageMultiplier(QSharedPointer<AttackMove> move) const;
    double speedMultiplier() const;
    bool canMove();


    void incrementTurnCounters();
    double getStatusDamageMultiplier(); // end turn damage from poison and burn
    void resetStatuses(); // clear confusion, etc. when switching out of battle


    Status getStatusCondition();
    void setStatusCondition(Status status_);
    bool getBurned() const;
    bool getFrozen() const;
    bool getParalyzed() const;
    bool getAsleep() const;
    bool getPoisoned() const;
    bool getBadlyPoisoned() const;
    bool getConfused() const;
    void setConfused(bool newConfused);
    bool getInfatuated() const;
    void setInfatuated(bool newInfatuated);
    int getBadPoisonTurn() const;
    void setBadPoisonTurn(int newBadPoisonTurn);
    int getMaxSleepTurns() const;
    void setMaxSleepTurns(int newMaxSleepTurns);
    int getNumTurnsAsleep() const;
    void setNumTurnsAsleep(int newNumTurnsAsleep);
    int getMaxConfusionTurns() const;
    void setMaxConfusionTurns(int newMaxConfusionTurns);
    int getNumTurnsConfused() const;
    void setNumTurnsConfused(int newNumTurnsConfused);

    QString toName();
    QString toColor();

    template <typename E>
    static constexpr typename std::underlying_type<E>::type to_underlying(E e) noexcept {
        return static_cast<typename std::underlying_type<E>::type>(e);
    }

private:

    bool burned;
    bool frozen;
    bool paralyzed;
    bool asleep;
    bool poisoned;
    bool badlyPoisoned;
    bool confused;
    bool infatuated;

    Status statusCondition;
    int badPoisonTurn; // number of turns under Status::BADLY_POISONED
    int maxSleepTurns;
    int numTurnsAsleep;
    int maxConfusionTurns;
    int numTurnsConfused;

    void setBurned(bool newBurned);
    void setFrozen(bool newFrozen);
    void setParalyzed(bool newParalyzed);
    void setAsleep(bool newAsleep);
    void setPoisoned(bool newPoisoned);
    void setBadlyPoisoned(bool newBadlyPoisoned);

signals:
    void hurtByConfusion();

};

#endif // STATUSCONDITION_H
