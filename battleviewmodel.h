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

private:
    QSharedPointer<Pokemon> currentPokemon;
    QSharedPointer<Trainer> player;
    QSharedPointer<Trainer> opponent;
};

#endif // BATTLEVIEWMODEL_H
