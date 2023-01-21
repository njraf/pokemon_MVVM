#ifndef POKEBALLITEM_H
#define POKEBALLITEM_H

#include <QObject>

#include "pokemon.h"

class PokeballItem : public QObject
{
    Q_OBJECT
public:
    explicit PokeballItem(QString name_, double catchRate_, int quantity_, QObject *parent = nullptr);
    ~PokeballItem() = default;

    QString getName();
    int getQuantity();
    void setQuantity(int quantity_);
    bool throwAtPokemon(QSharedPointer<Pokemon> pokemon);

private:
    QString name;
    double catchRate;
    int quantity;

signals:
    void usedLast();

};

#endif // POKEBALLITEM_H
