#ifndef POKEMON_H
#define POKEMON_H

#include <QString>
#include <QObject>
#include <QSharedPointer>

#include "typeutilities.h"

class Pokemon : public QObject
{
    Q_OBJECT
public:
    Pokemon(QString name_, int attackStat_, int spAttackStat_, int defenseStat_, int spDefenseStat_, int healthStat_, int speedStat_, int maxHealthStat_, int level_, Type type1_, Type type2_ = Type::NONE);
    ~Pokemon() = default;

    void attack(QSharedPointer<Pokemon> opponent);

    QString getName() const;
    void setName(const QString &newName);
    int getAttackStat() const;
    void setAttackStat(int newAttackStat);
    int getSpAttackStat() const;
    void setSpAttackStat(int newSpAttackStat);
    int getDefenseStat() const;
    void setDefenseStat(int newDefenseStat);
    int getSpDefenseStat() const;
    void setSpDefenseStat(int newSpDefenseStat);
    int getHealthStat() const;
    void setHealthStat(int newHealthStat);
    int getSpeedStat() const;
    void setSpeedStat(int newSpeedStat);
    int getMaxHealthStat() const;
    void setMaxHealthStat(int newMaxHealthStat);
    int getLevel() const;
    void setLevel(int newLevel);

    Type getType1() const;
    Type getType2() const;

private:
    QString name;
    int attackStat;
    int spAttackStat;
    int defenseStat;
    int spDefenseStat;
    int healthStat;
    int speedStat;
    int maxHealthStat;
    int level;
    Type type1;
    Type type2;
};

#endif // POKEMON_H
