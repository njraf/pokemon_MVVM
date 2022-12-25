#ifndef BATTLEVIEWMODEL_H
#define BATTLEVIEWMODEL_H

#include <QObject>

#include <QSharedPointer>

#include "pokemon.h"
#include "trainer.h"

class BattleViewmodel : public QObject
{
    Q_OBJECT
public:
    BattleViewmodel(QSharedPointer<Trainer> player_, QSharedPointer<Trainer> opponent_);
    ~BattleViewmodel() = default;
    QSharedPointer<Trainer> getPlayerTrainer();
    QSharedPointer<Trainer> getOpponentTrainer();
    QSharedPointer<Pokemon> getCurrentPlayerPokemon();
    QSharedPointer<Pokemon> getCurrentOpponentPokemon();

    void summonFirstPokemon();
    void attack(int attackIndex);

private:
    QSharedPointer<Trainer> player;
    QSharedPointer<Trainer> opponent;
    QSharedPointer<Pokemon> currentPlayerPokemon;
    QSharedPointer<Pokemon> currentOpponentPokemon;

signals:
    void summonedPokemon(QSharedPointer<Pokemon> playerPokemon, QSharedPointer<Pokemon> opponentPokemon);
    void stateUpdated();
};

#endif // BATTLEVIEWMODEL_H
