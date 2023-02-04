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
    case 4: // poison point
        ability = [](QSharedPointer<Pokemon> target) {
            if ((target->getStatusCondition()->getStatusCondition() != Status::NONE) ||
                    (target->getType1() == Type::POISON) || (target->getType2() == Type::POISON) ||
                    (target->getType1() == Type::STEEL) || (target->getType2() == Type::STEEL))
            {
                return;
            }

            const int CHANCE = 30; // 30%
            if ((QRandomGenerator::global()->generate() % 100) >= CHANCE) {
                return;
            }
            target->getStatusCondition()->setStatusCondition(Status::POISONED);
            qDebug() << target->getName() << "was poisoned by Poison Point ability";
        };
        break;
    case 5: // effect spore
        ability = [](QSharedPointer<Pokemon> target) {
            if ((target->getStatusCondition()->getStatusCondition() != Status::NONE) ||
                    (target->getType1() == Type::GRASS) || (target->getType2() == Type::GRASS))
            {
                return;
            }

            const int chance = (QRandomGenerator::global()->generate() % 100);
            if ((0 <= chance) && (chance <= 8)) {
                target->getStatusCondition()->setStatusCondition(Status::POISONED);
                qDebug() << target->getName() << "was poisoned by Effect Spore ability";
            } else if ((9 <= chance) && (chance <= 18)) {
                target->getStatusCondition()->setStatusCondition(Status::PARALYZED);
                qDebug() << target->getName() << "was paralyzed by Effect Spore ability";
            } else if ((19 <= chance) && (chance <= 29)) {
                target->getStatusCondition()->setStatusCondition(Status::ASLEEP);
                qDebug() << target->getName() << "was put to sleep by Effect Spore ability";
            }
        };
        break;
    default:
        ability = [](QSharedPointer<Pokemon> target) {};
        break;
    }

    return ability;
}
