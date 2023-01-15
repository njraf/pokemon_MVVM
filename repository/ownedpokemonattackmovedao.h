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
    QVector<QSharedPointer<Pokemon>> getPartyPokemon();
    QVector<QSharedPointer<Pokemon>> getPokemonFromBox(int box);

protected:
    bool populateDatabase() override;

private:
    QVector<QSharedPointer<AttackMove>> populateAttackList(const QSqlQueryModel &attackModel);
    QSharedPointer<Pokemon> makePokemon(const QSqlRecord &record, const QVector<QSharedPointer<AttackMove>> &attackList);
};

#endif // OWNEDPOKEMONATTACKMOVEDAO_H
