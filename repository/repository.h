#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QObject>

#include "pokemon.h"

#include "pokemondao.h"
#include "attackmovedao.h"

class Repository : public QObject
{
    Q_OBJECT
public:
    explicit Repository(QSharedPointer<PokemonDao> pokemonDao_, QSharedPointer<AttackMoveDao> attackMoveDao_, QObject *parent = nullptr);
    ~Repository();

    bool hasConnection();
    QSharedPointer<Pokemon> getPokemon(int nationalDexNumber);
    QSharedPointer<AttackMove> getAttackByID(int id);

private:
    QSqlDatabase db;
    QSharedPointer<PokemonDao> pokemonDao;
    QSharedPointer<AttackMoveDao> attackMoveDao;

signals:

};

#endif // REPOSITORY_H
