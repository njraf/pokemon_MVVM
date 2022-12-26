#ifndef ABILITY_H
#define ABILITY_H

#include <QObject>
#include <QSharedPointer>

class Pokemon;

enum class BattleStage : int {
    SUMMON, BEFORE_ATTACK, AFTER_ATTACK, ATTACK_HITS, OPPONENT_HITS, START_TURN, END_TURN, FAINT
};

class Ability : public QObject
{
    Q_OBJECT
public:
    explicit Ability(BattleStage stage_, std::function<void(QSharedPointer<Pokemon>, QSharedPointer<Pokemon>)> ability_, QObject *parent = nullptr);
    ~Ability() = default;
    Ability(const Ability &o);

    void useAbility(QSharedPointer<Pokemon> playerPokemon, QSharedPointer<Pokemon> opponentPokemon);
    BattleStage getBattleStage();

    Ability& operator=(Ability o);

private:
    BattleStage battleStage;
    std::function<void(QSharedPointer<Pokemon>, QSharedPointer<Pokemon>)> ability;


signals:

};

#endif // ABILITY_H
