#include "abilityfactory.h"

#include <QDebug>
#include <QRandomGenerator>

AbilityFactory::AbilityFactory(QObject *parent) : QObject(parent)
{

}

std::function<void(QSharedPointer<Pokemon>)> AbilityFactory::getAbility(int id) {
    std::function<void(QSharedPointer<Pokemon>)> ability;
    switch (id) {
    case 1: // intimidate
        ability = [](QSharedPointer<Pokemon> target) {
            target->setAttackStatStage(target->getAttackStatStage() - 1);
            qDebug() << target->getName() << "had it's attack drop one stage";
        };
        break;
    case 2: // aftermath
        ability = [](QSharedPointer<Pokemon> target) {
            int damage = 0.25 * static_cast<double>(target->getMaxHealthStat());
            target->setHealthStat(target->getHealthStat() - damage);
            qDebug() << "Aftermath dealt" << damage << "damage to" << target->getName();
        };
        break;
    case 3: // static
        ability = [](QSharedPointer<Pokemon> target) {
            if (target->getStatusCondition()->getStatusCondition() != Status::NONE) {
                return;
            }
            const int CHANCE = 30; // 30%
            if ((QRandomGenerator::global()->generate() % 100) >= CHANCE) {
                return;
            }
            target->getStatusCondition()->setStatusCondition(Status::PARALYZED);
            qDebug() << target->getName() << "was paralyzed by Static ability";
        };
        break;
    default:
        ability = [](QSharedPointer<Pokemon> target) {};
        break;
    }

    return ability;
}
