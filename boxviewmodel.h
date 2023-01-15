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
    explicit BoxViewmodel(QSharedPointer<Repository> repository_, QVector<QSharedPointer<Pokemon>> partyPokemon, QObject *parent = nullptr);
    ~BoxViewmodel() = default;

    QVector<QSharedPointer<Pokemon>> getAllPokemonFromBox(int box);
    QVector<QSharedPointer<Pokemon>> getPartyPokemon();
    int getCurrentBox();
    void setCurrentBox(int box);

    const int ROWS = 5;
    const int COLS = 5;

private:
    const int MAX_BOX = 8;
    QSharedPointer<Repository> repository;
    QVector<QSharedPointer<Pokemon>> partyPokemon;
    int currentBox;

signals:

};

#endif // BOXVIEWMODEL_H
