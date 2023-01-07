#ifndef POKEBALLITEM_H
#define POKEBALLITEM_H

#include <QObject>

#include "pokemon.h"

class PokeballItem : public QObject
{
    Q_OBJECT
public:
    explicit PokeballItem(QString name_, double catchRate_, QObject *parent = nullptr);
    ~PokeballItem() = default;

    bool throwAtPokemon(QSharedPointer<Pokemon> pokemon);

private:
    QString name;
    double catchRate;

};

#endif // POKEBALLITEM_H
