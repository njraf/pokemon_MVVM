#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QObject>

#include "pokemon.h"

#include "pokemondao.h"
#include "attackmovedao.h"
#include "healitemdao.h"
#include "overworlddao.h"
#include "ownedpokemondao.h"
#include "ownedpokemonattackmovedao.h"

class Repository : public QObject
{
    Q_OBJECT
public:
    explicit Repository(QSharedPointer<PokemonDao> pokemonDao_, QSharedPointer<AttackMoveDao> attackMoveDao_, QSharedPointer<HealItemDao> healItemDao_,
                        QSharedPointer<OverworldDao> overworldDao_, QSharedPointer<OwnedPokemonDao> ownedPokemonDao_, QSharedPointer<OwnedPokemonAttackMoveDao> ownedPokemonAttackMoveDao_, QObject *parent = nullptr);
    ~Repository();

    bool hasConnection();
    void addNewPokemon(QSharedPointer<Pokemon> pokemon); // catch or gift pokemon
    QSharedPointer<Pokemon> getNewPokemon(int nationalDexNumber); // create a new pokemon (i.e. wild encounter)
    QSharedPointer<Pokemon> getPokemonByID(int id); // get owned pokemon
    QVector<QSharedPointer<Pokemon>> getPartyPokemon();
    QVector<QSharedPointer<Pokemon>> getPokemonFromBox(int box);
    QSharedPointer<AttackMove> getAttackByID(int id, int effectID = 0);
    QSharedPointer<AttackMove> getAttackByName(QString name, int effectID = 0);
    QSharedPointer<HealItem> getHealItemByID(int id);
    QVector<QVector<QSharedPointer<Tile>>> getMapByID(int id);

private:
    QSqlDatabase db;
    QSharedPointer<PokemonDao> pokemonDao;
    QSharedPointer<AttackMoveDao> attackMoveDao;
    QSharedPointer<HealItemDao> healItemDao;
    QSharedPointer<OverworldDao> overworldDao;
    QSharedPointer<OwnedPokemonDao> ownedPokemonDao;
    QSharedPointer<OwnedPokemonAttackMoveDao> ownedPokemonAttackMoveDao;

};

#endif // REPOSITORY_H
