#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QObject>

#include "pokemon.h"

#include "pokemondao.h"
#include "attackmovedao.h"
#include "healitemdao.h"
#include "overworlddao.h"
#include "pokeballitemdao.h"

class Repository : public QObject
{
    Q_OBJECT
public:
    explicit Repository(QSharedPointer<PokemonDao> pokemonDao_, QSharedPointer<AttackMoveDao> attackMoveDao_, QSharedPointer<HealItemDao> healItemDao_,
                        QSharedPointer<OverworldDao> overworldDao_, QSharedPointer<PokeballItemDao> pokeballDao_, QObject *parent = nullptr);
    ~Repository();

    bool hasConnection();
    QSharedPointer<Pokemon> getPokemon(int nationalDexNumber);
    QSharedPointer<AttackMove> getAttackByID(int id, int effectID = 0);
    QSharedPointer<AttackMove> getAttackByName(QString name, int effectID = 0);
    QSharedPointer<HealItem> getHealItemByID(int id);
    QVector<QVector<QSharedPointer<Tile>>> getMapByID(int id);
    QSharedPointer<PokeballItem> getPokeballItemByID(int id);

private:
    QSqlDatabase db;
    QSharedPointer<PokemonDao> pokemonDao;
    QSharedPointer<AttackMoveDao> attackMoveDao;
    QSharedPointer<HealItemDao> healItemDao;
    QSharedPointer<OverworldDao> overworldDao;
    QSharedPointer<PokeballItemDao> pokeballDao;

};

#endif // REPOSITORY_H
