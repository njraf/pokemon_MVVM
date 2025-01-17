#include "trainer.h"

#include <QDebug>

Trainer::Trainer(QSharedPointer<QVector<QSharedPointer<Pokemon>>> team_, QSharedPointer<Bag> bag_)
    : team(team_)
    , bag(bag_)
{

}

QSharedPointer<QVector<QSharedPointer<Pokemon>>> Trainer::getTeam() {
    return team;
}

QSharedPointer<Bag> Trainer::getBag() {
    return bag;
}

QString Trainer::getName() {
    return name;
}

