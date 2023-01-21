#include "abilityfactory.h"

#include <QDebug>

AbilityFactory::AbilityFactory(QObject *parent) : QObject(parent)
{

}

std::function<void(QSharedPointer<Pokemon>)> AbilityFactory::getAbility(int id) {
    std::function<void(QSharedPointer<Pokemon>)> ability;
    switch (id) {
    case 0: // intimidate
        ability = [](QSharedPointer<Pokemon> target) {
            target->setAttackStatStage(target->getAttackStatStage() - 1);
        };
        break;
    case 1: // aftermath
        ability = [](QSharedPointer<Pokemon> target) {
            target->setHealthStat(target->getHealthStat() - (0.25 * target->getMaxHealthStat())); //TODO: only activate when defeated by a physical move
            qDebug() << "Aftermath";
        };
        break;
    default:
        ability = [](QSharedPointer<Pokemon> target) {};
        break;
    }

    return ability;
}
