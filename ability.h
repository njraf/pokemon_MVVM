#ifndef ABILITY_H
#define ABILITY_H

#include <QObject>
#include <QSharedPointer>
#include <QMap>

#include "attackmove.h"

class Pokemon;

enum class BattleStage : int {
    SWITCH_IN, SWITCH_OUT, BEFORE_ATTACK, AFTER_ATTACK, BEFORE_OPPONENT_ATTACK, AFTER_OPPONENT_ATTACK, ATTACK_HITS, OPPONENT_HITS,
    START_TURN, END_TURN, FAINT, OPPONENT_FAINT, STATUS_APPLIED, STATUS_REMOVED, FLINCH
};

class Ability : public QObject
{
    Q_OBJECT

public:
    enum class Target : int {
        SELF, ALLY, OPPONENT
    };
    static QMap<QString, BattleStage> strToBattleStage;
    static QMap<QString, Target> strToTarget;

    /*!
     * \brief Ability The pokemon's ability
     * \param id_ The id of the ability
     * \param name_ The name of the ability
     * \param stages_ When the ability should activate
     * \param target_ Who the ability should target
     * \param attackCategory_ The attack category the attacker's attack move that activates this ability
     * \param attackType_ The attack type the attacker's attack move that activates this ability
     * \param ability_ The function that applies the ability
     * \param parent Parent QObject
     */
    explicit Ability(int id_, QString name_, QVector<BattleStage> stages_, Target target_, Category attackCategory_, Type attackType_, std::function<void(QSharedPointer<Pokemon>)> ability_, QObject *parent = nullptr);
    ~Ability() = default;
    Ability(const Ability &o);

    void useAbility(QSharedPointer<Pokemon> abilityUser, QSharedPointer<Pokemon> opponentPokemon, BattleStage stage, Category attackCategory_, Type attackType_);
    QVector<BattleStage> getBattleStages();

    Ability& operator=(Ability o);

private:
    int id;
    QString name;
    QVector<BattleStage> battleStages;
    Target target;
    Category attackCategory;
    Type attackType;
    std::function<void(QSharedPointer<Pokemon>)> ability;


signals:

};

#endif // ABILITY_H
