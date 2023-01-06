#include "attackeffectfactory.h"

AttackEffectFactory::AttackEffectFactory(QObject *parent) : QObject(parent)
{

}

std::function<void(QSharedPointer<Pokemon> self, QSharedPointer<Pokemon> opponent)> AttackEffectFactory::getEffectByID(int id) {
    switch (id) {
    case 4: // toxic
        return [](QSharedPointer<Pokemon> self, QSharedPointer<Pokemon> opponent) {
            opponent->getStatusCondition()->setStatusCondition(Status::BADLY_POISONED);
        };
    default:
        return [](QSharedPointer<Pokemon> self, QSharedPointer<Pokemon> opponent) {};
    }
}
