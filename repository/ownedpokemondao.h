#ifndef ownedpokemondao_h
#define ownedpokemondao_h

#include <QObject>
#include <QSharedPointer>

#include "idao.h"
#include "pokemon.h"
#include "attackmove.h"

class OwnedPokemonDao : public IDao
{
    Q_OBJECT
public:
    explicit OwnedPokemonDao(QObject *parent = nullptr);
    ~OwnedPokemonDao() = default;

    void insertPokemon(QSharedPointer<Pokemon> pokemon);

protected:
    bool populateDatabase() override;
};

#endif // ownedpokemondao_h
