#include "boxviewmodel.h"

BoxViewmodel::BoxViewmodel(QSharedPointer<Repository> repository_,QObject *parent)
    : QObject(parent)
    , repository(repository_)
    , currentBox(1)
{

}

QVector<QSharedPointer<Pokemon>> BoxViewmodel::getAllPokemonFromBox(int box) {
    return repository->getAllPokemonFromBox(box);
}

QVector<QSharedPointer<Pokemon>> BoxViewmodel::getPartyPokemon() {
    return repository->getPartyPokemon();
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
