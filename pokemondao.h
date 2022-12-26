#ifndef POKEMONDAO_H
#define POKEMONDAO_H

#include <QObject>
#include <QtSql>

#include "pokemon.h"
#include "idao.h"

class PokemonDao : public IDao
{
    Q_OBJECT
public:
    explicit PokemonDao(QObject *parent = nullptr);
    ~PokemonDao() = default;

    QSharedPointer<Pokemon> getPokemon(int nationalDexNumber);
    bool populateDatabase() override;

signals:

};

#endif // POKEMONDAO_H
