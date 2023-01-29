#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QObject>

#include "pokemon.h"

#include "pokemondao.h"
#include "attackmovedao.h"
#include "healitemdao.h"
#include "overworlddao.h"
#include "pokeballitemdao.h"
#include "abilitydao.h"

class Repository : public QObject
{
    Q_OBJECT
public:
    explicit Repository(QSharedPointer<PokemonDao> pokemonDao_, QSharedPointer<AttackMoveDao> attackMoveDao_, QSharedPointer<HealItemDao> healItemDao_,
                        QSharedPointer<OverworldDao> overworldDao_, QSharedPointer<PokeballItemDao> pokeballDao_, QSharedPointer<AbilityDao> abilityDao_, QObject *parent = nullptr);
    ~Repository();

    bool hasConnection();
    QSharedPointer<Pokemon> getPokemon(int nationalDexNumber);
    QSharedPointer<AttackMove> getAttackByID(int id, int effectID = 0);
    QSharedPointer<AttackMove> getAttackByName(QString name, int effectID = 0);
    QSharedPointer<HealItem> getHealItemByID(int id);
    QVector<QVector<QSharedPointer<Tile>>> getMapByID(int id);
    QSharedPointer<PokeballItem> getPokeballItemByID(int id);
    Ability getAbilityByID(int id);

private:
    QSqlDatabase db;
    QSharedPointer<PokemonDao> pokemonDao;
    QSharedPointer<AttackMoveDao> attackMoveDao;
    QSharedPointer<HealItemDao> healItemDao;
    QSharedPointer<OverworldDao> overworldDao;
    QSharedPointer<PokeballItemDao> pokeballDao;
    QSharedPointer<AbilityDao> abilityDao;

};

#endif // REPOSITORY_H
