#ifndef ABILITY_H
#define ABILITY_H

#include <QObject>
#include <QSharedPointer>
#include <QMap>

class Pokemon;

enum class BattleStage : int {
    SUMMON, BEFORE_ATTACK, AFTER_ATTACK, BEFORE_OPPONENT_ATTACK, AFTER_OPPONENT_ATTACK, ATTACK_HITS, OPPONENT_HITS, START_TURN, END_TURN, FAINT
}; //TODO: use OPPONENT_HITS

class Ability : public QObject
{
    Q_OBJECT

public:
    enum class Target : int {
        SELF, ALLY, OPPONENT
    };
    static QMap<QString, BattleStage> strToBattleStage;
    static QMap<QString, Target> strToTarget;

    explicit Ability(int id_, QString name_, BattleStage stage_, Target target_, std::function<void(QSharedPointer<Pokemon>)> ability_, QObject *parent = nullptr);
    ~Ability() = default;
    Ability(const Ability &o);

    void useAbility(QSharedPointer<Pokemon> abilityUser, QSharedPointer<Pokemon> opponentPokemon, BattleStage stage_);
    BattleStage getBattleStage();

    Ability& operator=(Ability o);

private:
    int id;
    QString name;
    BattleStage battleStage;
    Target target;
    std::function<void(QSharedPointer<Pokemon>)> ability;


signals:

};

#endif // ABILITY_H
