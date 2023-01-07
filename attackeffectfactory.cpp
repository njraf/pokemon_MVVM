#include "attackeffectfactory.h"

#include <QDebug>

AttackEffectFactory::AttackEffectFactory(QObject *parent) : QObject(parent)
{

}

std::function<void(QSharedPointer<Pokemon> self, QSharedPointer<Pokemon> opponent)> AttackEffectFactory::getEffectByID(int id) {
    switch (id) { // case 0 should default
    case 1: // apply BADLY POISONED status condition
        return [](QSharedPointer<Pokemon> self, QSharedPointer<Pokemon> opponent) {
            opponent->getStatusCondition()->setStatusCondition(Status::BADLY_POISONED);
        };
    case 2: // increase self ATTACK 2 stages
        return [](QSharedPointer<Pokemon> self, QSharedPointer<Pokemon> opponent) {
            self->setAttackStatStage(self->getAttackStatStage() + 2);
            qDebug() << self->getName() << "raised its attack by 2 stages. Attack stages" << self->getAttackStatStage();
        };
    case 3: // set CONFUSION status on opponent
        return [](QSharedPointer<Pokemon> self, QSharedPointer<Pokemon> opponent) {
            opponent->getStatusCondition()->setConfused(true);
            qDebug() << opponent->getName() << "is now confused";
        };
    case 4: // set SLEEP status on opponent
        return [](QSharedPointer<Pokemon> self, QSharedPointer<Pokemon> opponent) {
            opponent->getStatusCondition()->setStatusCondition(Status::ASLEEP);
            qDebug() << opponent->getName() << "fell asleep";
        };
    default: // do nothing
        return [](QSharedPointer<Pokemon> self, QSharedPointer<Pokemon> opponent) {};
    }
}
