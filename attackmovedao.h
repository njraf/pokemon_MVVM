#ifndef ATTACKMOVEDAO_H
#define ATTACKMOVEDAO_H

#include <QObject>

#include "idao.h"
#include "attackmove.h"

class AttackMoveDao : public IDao
{
    Q_OBJECT
public:
    explicit AttackMoveDao(QObject *parent = nullptr);
    ~AttackMoveDao() = default;

    QSharedPointer<AttackMove> getAttackByID(int id);
    bool populateDatabase() override;

signals:

};

#endif // ATTACKMOVEDAO_H