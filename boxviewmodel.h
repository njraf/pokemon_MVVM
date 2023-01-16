#ifndef BOXVIEWMODEL_H
#define BOXVIEWMODEL_H

#include <QObject>
#include <QSharedPointer>

#include "repository.h"
#include "pokemon.h"

class BoxViewmodel : public QObject
{
    Q_OBJECT
public:
    explicit BoxViewmodel(QSharedPointer<Repository> repository_, QSharedPointer<QVector<QSharedPointer<Pokemon>>> partyPokemon, QObject *parent = nullptr);
    ~BoxViewmodel() = default;

    QVector<QSharedPointer<Pokemon>> getAllPokemonFromBox(int box);
    QSharedPointer<QVector<QSharedPointer<Pokemon>>> getPartyPokemon();
    int getCurrentBox();
    void setCurrentBox(int box);
    void swapPartyPosition(QSharedPointer<Pokemon> first, QSharedPointer<Pokemon> second);

    const int ROWS = 5;
    const int COLS = 5;

private:
    const int MAX_BOX = 8;
    QSharedPointer<Repository> repository;
    QSharedPointer<QVector<QSharedPointer<Pokemon>>> partyPokemon;
    int currentBox;

signals:

};

#endif // BOXVIEWMODEL_H
