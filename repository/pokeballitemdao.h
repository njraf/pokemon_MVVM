#ifndef POKEBALLITEMDAO_H
#define POKEBALLITEMDAO_H

#include <QObject>

#include "idao.h"
#include "pokeballitem.h"

class PokeballItemDao : public IDao
{
    Q_OBJECT
public:
    explicit PokeballItemDao(QObject *parent = nullptr);
    ~PokeballItemDao() = default;

    QSharedPointer<PokeballItem> getPokeballItemByID(int id);
    bool populateDatabase();

};

#endif // POKEBALLITEMDAO_H
