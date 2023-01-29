#ifndef HEALITEMDAO_H
#define HEALITEMDAO_H

#include <QObject>

#include "idao.h"
#include "healitem.h"

class HealItemDao : public IDao
{
    Q_OBJECT
public:
    explicit HealItemDao(QObject *parent = nullptr);
    ~HealItemDao() = default;

    QSharedPointer<HealItem> getHealItemByID(int id);

protected:
    bool populateDatabase() override;
};

#endif // HEALITEMDAO_H
