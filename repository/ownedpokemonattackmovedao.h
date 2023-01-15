#ifndef OWNEDPOKEMONATTACKMOVEDAO_H
#define OWNEDPOKEMONATTACKMOVEDAO_H

#include <QObject>

#include "idao.h"
#include "pokemon.h"

class OwnedPokemonAttackMoveDao : public IDao
{
    Q_OBJECT
public:
    explicit OwnedPokemonAttackMoveDao(QObject *parent = nullptr);
    ~OwnedPokemonAttackMoveDao() = default;

    QSharedPointer<Pokemon> getPokemonByID(int id);
    void insertPokemon(QSharedPointer<Pokemon> pokemon);

protected:
    bool populateDatabase() override;
};

#endif // OWNEDPOKEMONATTACKMOVEDAO_H
