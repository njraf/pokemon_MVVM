#ifndef POKEMON_H
#define POKEMON_H

#include <QString>
#include <QObject>
#include <QSharedPointer>
#include <QtMath>

#include "typeutilities.h"
#include "attackmove.h"

class Pokemon : public QObject
{
    Q_OBJECT
public:
    Pokemon(QString name_, QString owner_, int baseMaxHealthStat_, int baseAttackStat_, int baseDefenseStat_, int baseSpAttackStat_, int baseSpDefenseStat_, int baseSpeedStat_, int currentHealthStat_, int level_, QVector<QSharedPointer<AttackMove>> attackList_, Type type1_, Type type2_ = Type::NONE);
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
    int getLevel() const;
    void setLevel(int newLevel);
    QVector<QSharedPointer<AttackMove> > getAttackList() const;
    Type getType1() const;
    Type getType2() const;

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

    QString name;
    QString owner;
    int currentHealthStat;
    int level;
    QVector<QSharedPointer<AttackMove>> attackList;
    Type type1;
    Type type2;

signals:
    void attacked();
};

#endif // POKEMON_H
