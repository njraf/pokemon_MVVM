#ifndef ABILITYFACTORY_H
#define ABILITYFACTORY_H

#include <QObject>

#include "pokemon.h"

class AbilityFactory : public QObject
{
    Q_OBJECT
public:
    static std::function<void(QSharedPointer<Pokemon>)> getAbility(int id);

private:
    explicit AbilityFactory(QObject *parent = nullptr);

signals:

};

#endif // ABILITYFACTORY_H
