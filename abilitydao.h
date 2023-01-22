#ifndef ABILITYDAO_H
#define ABILITYDAO_H

#include <QObject>

#include "idao.h"
#include "ability.h"
#include "abilityfactory.h"

class AbilityDao : public IDao
{
    Q_OBJECT
public:
    explicit AbilityDao(QObject *parent = nullptr);

    Ability getAbilityByID(int id);

protected:
    bool populateDatabase() override;
};

#endif // ABILITYDAO_H
