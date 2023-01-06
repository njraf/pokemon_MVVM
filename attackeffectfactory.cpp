#include "attackeffectfactory.h"

#include <QDebug>

AttackEffectFactory::AttackEffectFactory(QObject *parent) : QObject(parent)
{

}

std::function<void(QSharedPointer<Pokemon> self, QSharedPointer<Pokemon> opponent)> AttackEffectFactory::getEffectByID(int id) {
    switch (id) { // case 0 should default
    case 1: // apply bad poisoned status condition
        return [](QSharedPointer<Pokemon> self, QSharedPointer<Pokemon> opponent) {
            opponent->getStatusCondition()->setStatusCondition(Status::BADLY_POISONED);
        };
    case 2: // increase attack 2 stages
        return [](QSharedPointer<Pokemon> self, QSharedPointer<Pokemon> opponent) {
            self->setAttackStatStage(self->getAttackStatStage() + 2);
            qDebug() << self->getName() << "Raised its attack by 2 stages. Attack stages" << self->getAttackStatStage();
        };
    default: // do nothing
        return [](QSharedPointer<Pokemon> self, QSharedPointer<Pokemon> opponent) {};
    }
}
