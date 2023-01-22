#ifndef ABILITYDAO_H
#define ABILITYDAO_H

#include <QObject>

#include "idao.h"

class AbilityDao : public IDao
{
    Q_OBJECT
public:
    explicit AbilityDao(QObject *parent = nullptr);

protected:
    bool populateDatabase() override;
};

#endif // ABILITYDAO_H
