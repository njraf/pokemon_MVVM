#include "trainer.h"

Trainer::Trainer(QVector<QSharedPointer<Pokemon>> team_)
    : team(team_)
{

}

QVector<QSharedPointer<Pokemon>> Trainer::getTeam() {
    return team;
}

QString Trainer::getName() {
    return name;
}
