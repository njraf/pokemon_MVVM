#ifndef ATTACKEFFECTFACTORY_H
#define ATTACKEFFECTFACTORY_H

#include <QObject>
#include <QSharedPointer>

#include "pokemon.h"

class AttackEffectFactory : public QObject
{
    Q_OBJECT
public:
    static std::function<void(QSharedPointer<Pokemon> self, QSharedPointer<Pokemon> opponent)> getEffectByID(int id);

private:
    explicit AttackEffectFactory(QObject *parent = nullptr);

signals:

};

#endif // ATTACKEFFECTFACTORY_H
