#ifndef POKEMON_H
#define POKEMON_H

#include <QString>
#include <QObject>
#include <QSharedPointer>
#include <QtMath>

#include "typeutilities.h"
#include "attackmove.h"
#include "natureutilities.h"
#include "commonenumerations.h"

class Pokemon : public QObject
{
    Q_OBJECT
public:
    Pokemon(QString name_, QString owner_, Nature nature_, int baseMaxHealthStat_, int baseAttackStat_, int baseDefenseStat_,
            int baseSpAttackStat_, int baseSpDefenseStat_, int baseSpeedStat_, int currentHealthStat_, int level_,
            QVector<QSharedPointer<AttackMove>> attackList_, Type type1_, Type type2_ = Type::NONE);
    ~Pokemon() = default;

    void attack(QSharedPointer<Pokemon> opponent, QSharedPointer<AttackMove> attackMove);

    QString getName() const;
    void setName(const QString &newName);
    int getAttackStat() const;
    int getSpAttackStat() const;
    int getDefenseStat() const;
    int getSpDefenseStat() const;
    int getHealthStat() const;
    void setHealthStat(int newHealthStat);
    int getSpeedStat() const;
    int getMaxHealthStat() const;

    int getAttackStatStage() const;
    void setAttackStatStage(int stage);
    int getSpAttackStatStage() const;
    void setSpAttackStatStage(int stage);
    int getDefenseStatStage() const;
    void setDefenseStatStage(int stage);
    int getSpDefenseStatStage() const;
    void setSpDefenseStatStage(int stage);
    int getSpeedStatStage() const;
    void setSpeedStatStage(int stage);
    int getAccuracyStatStage() const;
    void setAccuracyStatStage(int stage);
    int getEvasionStatStage() const;
    void setEvasionStatStage(int stage);
    void resetAllStages();

    int getLevel() const;
    void setLevel(int newLevel);
    QVector<QSharedPointer<AttackMove> > getAttackList() const;
    void setAttackList(QVector<QSharedPointer<AttackMove> > newAttackList);
    Type getType1() const;
    Type getType2() const;

    Status getStatusCondition();
    void setStatusCondition(Status status);
    bool isConfused();
    void isConfused(bool confused_);
    bool isInfatuated();
    void isInfatuated(bool infatuated_);
    int getBadlyPoisonedTurn();
    void setBadlyPoisonedTurn(int badlyPoisonedTurn_);
    int getMaxSleepTurns();
    void setMaxSleepTurns(int maxSleepTurns_);
    int getNumSleepTurns();
    void setNumSleepTurns(int sleepTurns_);
    int getMaxConfusionTurns();
    void setMaxConfusionTurns(int maxTurns_);
    int getNumConfusionTurns();
    void setNumConfusionTurns(int numTurns_);

private:
    int baseAttackStat;
    int baseSpAttackStat;
    int baseDefenseStat;
    int baseSpDefenseStat;
    int baseSpeedStat;
    int baseMaxHealthStat;

    int attackStatIV;
    int spAttackStatIV;
    int defenseStatIV;
    int spDefenseStatIV;
    int speedStatIV;
    int maxHealthStatIV;

    int attackStatEV;
    int spAttackStatEV;
    int defenseStatEV;
    int spDefenseStatEV;
    int speedStatEV;
    int maxHealthStatEV;

    int attackStatStage;
    int spAttackStatStage;
    int defenseStatStage;
    int spDefenseStatStage;
    int speedStatStage;
    int accuracyStatStage;
    int evasionStatStage;

    QString name;
    QString owner;
    Nature nature;
    int currentHealthStat;
    int level;
    QVector<QSharedPointer<AttackMove>> attackList;
    Type type1;
    Type type2;

    Status statusCondition;
    bool confused;
    bool infatuated; // under the effect of attract
    int badPoisonTurn; // number of turns under Status::BADLY_POISONED
    int maxSleepTurns;
    int numTurnsAsleep;
    int maxConfusionTurns;
    int numTurnsConfused;

    double getStatStageMultiplier(int stage) const;
    double getAccuracyStageMultiplier(int stage) const;
    double getEvasionStageMultiplier(int stage) const;

    int calculateDamage(QSharedPointer<Pokemon> opponent, QSharedPointer<AttackMove> attackMove);


signals:
    void attacked();
};

#endif // POKEMON_H
