#ifndef BATTLEVIEWMODEL_H
#define BATTLEVIEWMODEL_H

#include <QObject>

#include <QSharedPointer>

#include "pokemon.h"
#include "trainer.h"
#include "repository.h"

class BattleViewmodel : public QObject
{
    Q_OBJECT
public:
    BattleViewmodel(QSharedPointer<Repository> repository_, QSharedPointer<Trainer> player_);
    ~BattleViewmodel() = default;
    QSharedPointer<Trainer> getPlayerTrainer();
    QSharedPointer<Trainer> getOpponentTrainer();
    void setOpponentTrainer(QSharedPointer<Trainer> opponent_);
    QSharedPointer<Pokemon> getCurrentPlayerPokemon();
    QSharedPointer<Pokemon> getCurrentOpponentPokemon();

    void summonFirstPokemon();
    void playerSummon(QSharedPointer<Pokemon> pokemon);
    void opponentSummon(QSharedPointer<Pokemon> pokemon);
    void attack(int attackIndex);

private:
    QSharedPointer<Repository> repository;
    QSharedPointer<Trainer> player;
    QSharedPointer<Trainer> opponent;
    QSharedPointer<Pokemon> currentPlayerPokemon;
    QSharedPointer<Pokemon> currentOpponentPokemon;
    QVector<QSharedPointer<Pokemon>> fighters; // point to currentPlayerPokemon and currentOpponenetPokemon

    void resolveTurn(); // run at the end of each turn, after all attacks
    void summonPokemon(QSharedPointer<Pokemon> pokemon); // a generic summon method

private slots:
    void resolveAttack(); // run after each pokemon's attack

signals:
    void summonedPokemon(QSharedPointer<Pokemon> playerPokemon, QSharedPointer<Pokemon> opponentPokemon);
    void stateUpdated();
    void battleFinished(QString winLoseMessage);
};

#endif // BATTLEVIEWMODEL_H
