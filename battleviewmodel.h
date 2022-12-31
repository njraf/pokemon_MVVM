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

private slots:
    void afterAttacking();

signals:
    void summonedPokemon(QSharedPointer<Pokemon> playerPokemon, QSharedPointer<Pokemon> opponentPokemon);
    void stateUpdated();
    void battleFinished(QString winLoseMessage);
};

#endif // BATTLEVIEWMODEL_H
