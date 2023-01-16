#include "boxviewmodel.h"

#include <algorithm>

BoxViewmodel::BoxViewmodel(QSharedPointer<Repository> repository_, QSharedPointer<QVector<QSharedPointer<Pokemon>>> partyPokemon_, QObject *parent)
    : QObject(parent)
    , repository(repository_)
    , partyPokemon(partyPokemon_)
    , currentBox(1)
{

}

QVector<QSharedPointer<Pokemon>> BoxViewmodel::getAllPokemonFromBox(int box) {
    return repository->getAllPokemonFromBox(box);
}

QSharedPointer<QVector<QSharedPointer<Pokemon>>> BoxViewmodel::getPartyPokemon() {
    return partyPokemon;
}

int BoxViewmodel::getCurrentBox() {
    return currentBox;
}

void BoxViewmodel::setCurrentBox(int box) {
    if (box > MAX_BOX) {
        currentBox = 1;
    } else if (box < 1) {
        currentBox = MAX_BOX;
    } else {
        currentBox = box;
    }
}

void BoxViewmodel::swapPartyPosition(QSharedPointer<Pokemon> first, QSharedPointer<Pokemon> second) {
    auto firstIt = std::find_if(partyPokemon->begin(), partyPokemon->end(), [=](QSharedPointer<Pokemon> p) { return (p->getID() == first->getID()); });
    auto secondIt = std::find_if(partyPokemon->begin(), partyPokemon->end(), [=](QSharedPointer<Pokemon> p) { return (p->getID() == second->getID()); });

    if ((firstIt == partyPokemon->end()) || (secondIt == partyPokemon->end())) {
        return;
    }

    QSharedPointer<Pokemon> tmp = *firstIt;
    (*firstIt) = *secondIt;
    (*secondIt) = tmp;
}
