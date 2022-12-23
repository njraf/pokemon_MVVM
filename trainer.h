#ifndef TRAINER_H
#define TRAINER_H

#include <QObject>
#include <QSharedPointer>

#include "pokemon.h"

class Trainer : public QObject
{
    Q_OBJECT
public:
    Trainer(QVector<QSharedPointer<Pokemon>> team_);
    ~Trainer() = default;
    QVector<QSharedPointer<Pokemon>> getTeam();
    QString getName();

private:
    QVector<QSharedPointer<Pokemon>> team;
    QString name;
};

#endif // TRAINER_H
