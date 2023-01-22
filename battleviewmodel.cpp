#include "battleviewmodel.h"

#include <QRandomGenerator>

BattleViewmodel::BattleViewmodel(QSharedPointer<Repository> repository_, QSharedPointer<Trainer> player_, QSharedPointer<Trainer> opponent_)
    : repository(repository_)
    , player(player_)
    , opponent(opponent_)
{
    for (auto member : player->getTeam()) {
        connect(member.data(), &Pokemon::attacked, this, &BattleViewmodel::stateUpdated);
    }

    for (auto member : opponent->getTeam()) {
        connect(member.data(), &Pokemon::attacked, this, &BattleViewmodel::stateUpdated);
    }
}

QSharedPointer<Trainer> BattleViewmodel::getPlayerTrainer() {
    return player;
}

QSharedPointer<Trainer> BattleViewmodel::getOpponentTrainer() {
    return opponent;
}

QSharedPointer<Pokemon> BattleViewmodel::getCurrentPlayerPokemon() {
    return currentPlayerPokemon;
}

QSharedPointer<Pokemon> BattleViewmodel::getCurrentOpponentPokemon() {
    return currentOpponentPokemon;
}

void BattleViewmodel::summonFirstPokemon() {
    auto playerTeam = player->getTeam();
    auto pokeIt = std::find_if(playerTeam.begin(), playerTeam.end(), [](QSharedPointer<Pokemon> pokemon) { return (pokemon->getHealthStat() > 0); });
    if (pokeIt != playerTeam.end()) {
        currentPlayerPokemon = *pokeIt;
    }

    auto opponentTeam = opponent->getTeam();
    pokeIt = std::find_if(opponentTeam.begin(), opponentTeam.end(), [](QSharedPointer<Pokemon> pokemon) { return (pokemon->getHealthStat() > 0); });
    if (pokeIt != opponentTeam.end()) {
        currentOpponentPokemon = *pokeIt;
    }

    currentPlayerPokemon->resetAllStages();
    currentOpponentPokemon->resetAllStages();

    Ability ability = currentPlayerPokemon->getAbility();
    ability.useAbility(currentPlayerPokemon, currentOpponentPokemon, BattleStage::SWITCH_IN, Category::NONE, Type::NONE);

    ability = currentOpponentPokemon->getAbility();
    ability.useAbility(currentOpponentPokemon, currentPlayerPokemon, BattleStage::SWITCH_IN, Category::NONE, Type::NONE);

    emit summonedPokemon(currentPlayerPokemon, currentOpponentPokemon);
}

void BattleViewmodel::attack(int attackIndex) {
    if (currentPlayerPokemon->getAttackList().size() <= attackIndex) {
        return; // UI button is not mapped to an attack
    }

    int opponentAttackIndex = (QRandomGenerator::global()->generate() % currentOpponentPokemon->getAttackList().size()); // random opponent attack
    auto playerAttackMove = currentPlayerPokemon->getAttackList().at(attackIndex);
    auto opponentAttackMove = currentOpponentPokemon->getAttackList().at(opponentAttackIndex);

    currentPlayerPokemon->getAbility().useAbility(currentPlayerPokemon, currentOpponentPokemon, BattleStage::START_TURN, playerAttackMove->getCategory(), playerAttackMove->getType());
    currentOpponentPokemon->getAbility().useAbility(currentPlayerPokemon, currentOpponentPokemon, BattleStage::START_TURN, opponentAttackMove->getCategory(), opponentAttackMove->getType());

    if (currentPlayerPokemon->getSpeedStat() > currentOpponentPokemon->getSpeedStat()) {
        currentPlayerPokemon->attack(currentOpponentPokemon, playerAttackMove);
        if (currentOpponentPokemon->getHealthStat() > 0) {
            currentOpponentPokemon->attack(currentPlayerPokemon, opponentAttackMove);
        } else {
            currentOpponentPokemon->getAbility().useAbility(currentOpponentPokemon, currentPlayerPokemon, BattleStage::FAINT, playerAttackMove->getCategory(), playerAttackMove->getType());
        }
    } else if (currentPlayerPokemon->getSpeedStat() < currentOpponentPokemon->getSpeedStat()) {
        currentOpponentPokemon->attack(currentPlayerPokemon, opponentAttackMove);
        if (currentPlayerPokemon->getHealthStat() > 0) {
            currentPlayerPokemon->attack(currentOpponentPokemon, playerAttackMove);
        } else {
            currentPlayerPokemon->getAbility().useAbility(currentPlayerPokemon, currentOpponentPokemon, BattleStage::FAINT, playerAttackMove->getCategory(), playerAttackMove->getType());
        }
    } else { // speed tie
        if ((QRandomGenerator::global()->generate() % 2) == 0) {
            currentPlayerPokemon->attack(currentOpponentPokemon, playerAttackMove);
            if (currentOpponentPokemon->getHealthStat() > 0) {
                currentOpponentPokemon->attack(currentPlayerPokemon, opponentAttackMove);
            } else {
                currentOpponentPokemon->getAbility().useAbility(currentOpponentPokemon, currentPlayerPokemon, BattleStage::FAINT, playerAttackMove->getCategory(), playerAttackMove->getType());
            }
        } else {
            currentOpponentPokemon->attack(currentPlayerPokemon, opponentAttackMove);
            if (currentPlayerPokemon->getHealthStat() > 0) {
                currentPlayerPokemon->attack(currentOpponentPokemon, playerAttackMove);
            } else {
                currentPlayerPokemon->getAbility().useAbility(currentPlayerPokemon, currentOpponentPokemon, BattleStage::FAINT, playerAttackMove->getCategory(), playerAttackMove->getType());
            }
        }
    }

    currentPlayerPokemon->getAbility().useAbility(currentPlayerPokemon, currentOpponentPokemon, BattleStage::END_TURN, playerAttackMove->getCategory(), playerAttackMove->getType());
    currentOpponentPokemon->getAbility().useAbility(currentPlayerPokemon, currentOpponentPokemon, BattleStage::END_TURN, opponentAttackMove->getCategory(), opponentAttackMove->getType());

    emit stateUpdated();
}
