#include "battleviewmodel.h"
#include "pagenavigator.h"

#include <QRandomGenerator>

BattleViewmodel::BattleViewmodel(QSharedPointer<Repository> repository_, QSharedPointer<Trainer> player_)
    : repository(repository_)
    , player(player_)
{
    for (auto member : *player->getTeam()) {
        connect(member.data(), &Pokemon::attacked, this, &BattleViewmodel::resolveAttack);
        connect(member.data(), &Pokemon::tookDamage, this, &BattleViewmodel::stateUpdated);
    }
}

void BattleViewmodel::resolveAttack() {
    emit stateUpdated();
    // determine win/lose and emit signal
    auto playerTeam = player->getTeam();
    auto opponentTeam = opponent->getTeam();
    bool playerKOed = (0 == std::count_if(playerTeam->begin(), playerTeam->end(), [](QSharedPointer<Pokemon> pokemon) { return (pokemon->getHealthStat() > 0); }));
    bool opponentKOed = (0 == std::count_if(opponentTeam->begin(), opponentTeam->end(), [](QSharedPointer<Pokemon> pokemon) { return (pokemon->getHealthStat() > 0); }));

    if (playerKOed) { // lose even if tied
        emit battleFinished("You lost");
        return;
    } else if (opponentKOed) {
        emit battleFinished("You won");
        return;
    }

    // send another player pokemon out if fainted
    if (currentPlayerPokemon->getHealthStat() <= 0) {
        auto playerPokemon = QVariant::fromValue<QSharedPointer<Pokemon>>(currentPlayerPokemon);
        QVector<QVariant> data = {playerPokemon};
        PageNavigator::getInstance()->navigate(PageName::TEAM, data);
    }

    // send another opponent pokemon out if fainted
    if (currentOpponentPokemon->getHealthStat() <= 0) {
        QVector<QSharedPointer<Pokemon>> livingOpponentTeam(opponentTeam->size());
        auto teamIt = std::copy_if(opponentTeam->begin(), opponentTeam->end(), livingOpponentTeam.begin(), [](QSharedPointer<Pokemon> pokemon) { return (pokemon->getHealthStat() > 0); });
        livingOpponentTeam.resize(std::distance(livingOpponentTeam.begin(), teamIt));
        opponentSummon(livingOpponentTeam.at(QRandomGenerator::global()->generate() % livingOpponentTeam.size())); //TODO: change summon strategy
    }

    static int numAttacks = 0;
    numAttacks++;
    if (numAttacks == fighters.size()) {
        numAttacks = 0;
        resolveTurn();
    }
}

void BattleViewmodel::resolveTurn() {
    for (auto pokemon : fighters) {
        auto statusCondition = pokemon->getStatusCondition();
        pokemon->setHealthStat(pokemon->getHealthStat() - (pokemon->getMaxHealthStat() * statusCondition->getStatusDamageMultiplier()));
    }
    emit stateUpdated();
    // determine win/lose and emit signal
    auto playerTeam = player->getTeam();
    auto opponentTeam = opponent->getTeam();
    bool playerKOed = (0 == std::count_if(playerTeam->begin(), playerTeam->end(), [](QSharedPointer<Pokemon> pokemon) { return (pokemon->getHealthStat() > 0); }));
    bool opponentKOed = (0 == std::count_if(opponentTeam->begin(), opponentTeam->end(), [](QSharedPointer<Pokemon> pokemon) { return (pokemon->getHealthStat() > 0); }));

    if (playerKOed) { // lose even if tied
        emit battleFinished("You lost");
        return;
    } else if (opponentKOed) {
        emit battleFinished("You won");
        return;
    }
}

QSharedPointer<Trainer> BattleViewmodel::getPlayerTrainer() {
    return player;
}

QSharedPointer<Trainer> BattleViewmodel::getOpponentTrainer() {
    return opponent;
}

void BattleViewmodel::setOpponentTrainer(QSharedPointer<Trainer> opponent_) {
    opponent = opponent_;
    for (auto member : *opponent->getTeam()) {
        connect(member.data(), &Pokemon::attacked, this, &BattleViewmodel::resolveAttack);
        connect(member.data(), &Pokemon::tookDamage, this, &BattleViewmodel::stateUpdated);
    }
}

QSharedPointer<Pokemon> BattleViewmodel::getCurrentPlayerPokemon() {
    return currentPlayerPokemon;
}

QSharedPointer<Pokemon> BattleViewmodel::getCurrentOpponentPokemon() {
    return currentOpponentPokemon;
}

void BattleViewmodel::summonFirstPokemon() {
    auto playerTeam = player->getTeam();
    auto pokeIt = std::find_if(playerTeam->begin(), playerTeam->end(), [](QSharedPointer<Pokemon> pokemon) { return (pokemon->getHealthStat() > 0); });
    if (pokeIt != playerTeam->end()) {
        currentPlayerPokemon = *pokeIt;
    }

    auto opponentTeam = opponent->getTeam();
    pokeIt = std::find_if(opponentTeam->begin(), opponentTeam->end(), [](QSharedPointer<Pokemon> pokemon) { return (pokemon->getHealthStat() > 0); });
    if (pokeIt != opponentTeam->end()) {
        currentOpponentPokemon = *pokeIt;
    }

    summonPokemon(currentPlayerPokemon);
    summonPokemon(currentOpponentPokemon);
    fighters.append(currentPlayerPokemon);
    fighters.append(currentOpponentPokemon);

    emit summonedPokemon(currentPlayerPokemon, currentOpponentPokemon);
}

void BattleViewmodel::playerSummon(QSharedPointer<Pokemon> pokemon) {
    currentPlayerPokemon = pokemon;
    summonPokemon(currentPlayerPokemon);

    fighters[0] = currentPlayerPokemon;
    emit summonedPokemon(currentPlayerPokemon, currentOpponentPokemon);
    int opponentAttackIndex = (QRandomGenerator::global()->generate() % currentOpponentPokemon->getAttackList().size()); //TODO: change attack strategy
    currentOpponentPokemon->attack(currentPlayerPokemon, currentOpponentPokemon->getAttackList().at(opponentAttackIndex));
}

void BattleViewmodel::opponentSummon(QSharedPointer<Pokemon> pokemon) {
    currentOpponentPokemon = pokemon;
    summonPokemon(currentOpponentPokemon);

    fighters[1] = currentOpponentPokemon;
    emit summonedPokemon(currentPlayerPokemon, currentOpponentPokemon);
}

void BattleViewmodel::summonPokemon(QSharedPointer<Pokemon> pokemon) {
    connect(pokemon.data(), &Pokemon::statusConditionSet, this, &BattleViewmodel::stateUpdated);
    pokemon->resetAllStages();
    auto statusCondition = pokemon->getStatusCondition();
    if (statusCondition->getBadlyPoisoned()) {
        pokemon->setStatusCondition(Status::POISONED);
    }

    statusCondition->resetStatuses();
}

void BattleViewmodel::attack(int attackIndex) {
    if (currentPlayerPokemon->getAttackList().size() <= attackIndex) {
        return; // the attack button is not tied to any moves
    }

    for (auto f : fighters) { // counters for confusion, etc.
        f->getStatusCondition()->incrementTurnCounters();
    }

    QVector<QPair<QSharedPointer<Pokemon>,int>> turnOrder;
    int opponentAttackIndex = (QRandomGenerator::global()->generate() % currentOpponentPokemon->getAttackList().size()); //TODO: change attack strategy
    if (currentPlayerPokemon->getSpeedStat() > currentOpponentPokemon->getSpeedStat()) {
        turnOrder.append(QPair<QSharedPointer<Pokemon>,int>(currentPlayerPokemon, attackIndex));
        turnOrder.append(QPair<QSharedPointer<Pokemon>,int>(currentOpponentPokemon, opponentAttackIndex));
    } else if (currentPlayerPokemon->getSpeedStat() < currentOpponentPokemon->getSpeedStat()) {
        turnOrder.append(QPair<QSharedPointer<Pokemon>,int>(currentOpponentPokemon, opponentAttackIndex));
        turnOrder.append(QPair<QSharedPointer<Pokemon>,int>(currentPlayerPokemon,attackIndex));
    } else { // speed tie
        if ((QRandomGenerator::global()->generate() % 2) == 0) {
            turnOrder.append(QPair<QSharedPointer<Pokemon>,int>(currentPlayerPokemon, attackIndex));
            turnOrder.append(QPair<QSharedPointer<Pokemon>,int>(currentOpponentPokemon, opponentAttackIndex));
        } else {
            turnOrder.append(QPair<QSharedPointer<Pokemon>,int>(currentOpponentPokemon, opponentAttackIndex));
            turnOrder.append(QPair<QSharedPointer<Pokemon>,int>(currentPlayerPokemon, attackIndex));
        }
    }

    turnOrder[0].first->attack(turnOrder[1].first, turnOrder[0].first->getAttackList().at(turnOrder[0].second));
    if (turnOrder[1].first->getHealthStat() > 0) {
        turnOrder[1].first->attack(turnOrder[0].first, turnOrder[1].first->getAttackList().at(turnOrder[1].second));
    }
}
