#ifndef POKEMON_H
#define POKEMON_H

#include <QString>
#include <QObject>

class Pokemon : public QObject
{
    Q_OBJECT
public:
    Pokemon(QString name_, int attackStat_, int spAttackStat_, int defenceStat_, int spDefenceStat_, int healthStat_, int speedStat_);
    ~Pokemon() = default;
    QString getName();

private:
    QString name;
    int attackStat;
    int spAttackStat;
    int defenceStat;
    int spDefenceStat;
    int healthStat;
    int speedStat;
};

#endif // POKEMON_H
