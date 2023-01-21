#ifndef ABILITY_H
#define ABILITY_H

#include <QObject>
#include <QSharedPointer>

class Pokemon;

enum class BattleStage : int {
    SUMMON, BEFORE_ATTACK, AFTER_ATTACK, BEFORE_OPPONENT_ATTACK, AFTER_OPPONENT_ATTACK, ATTACK_HITS, OPPONENT_HITS, START_TURN, END_TURN, FAINT
};

class Ability : public QObject
{
    Q_OBJECT

public:
    enum class Target : int {
        SELF, ALLY, OPPONENT
    };

    explicit Ability(BattleStage stage_, Target target_, std::function<void(QSharedPointer<Pokemon>)> ability_, QObject *parent = nullptr);
    ~Ability() = default;
    Ability(const Ability &o);

    void useAbility(QSharedPointer<Pokemon> abilityUser, QSharedPointer<Pokemon> opponentPokemon, BattleStage stage_);
    BattleStage getBattleStage();

    Ability& operator=(Ability o);

private:
    BattleStage battleStage;
    Target target;
    std::function<void(QSharedPointer<Pokemon>)> ability;


signals:

};

#endif // ABILITY_H
