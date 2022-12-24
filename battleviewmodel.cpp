#include "battleviewmodel.h"

BattleViewmodel::BattleViewmodel(QSharedPointer<Trainer> player_, QSharedPointer<Trainer> opponent_)
    : player(player_)
    , opponent(opponent_)
{

}

QSharedPointer<Trainer> BattleViewmodel::getPlayerTrainer() {
    return player;
}

QSharedPointer<Trainer> BattleViewmodel::getOpponentTrainer() {
    return opponent;
}

