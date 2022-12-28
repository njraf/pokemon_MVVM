#ifndef TRAINER_H
#define TRAINER_H

#include <QObject>
#include <QSharedPointer>

#include "pokemon.h"
#include "bag.h"

class Trainer : public QObject
{
    Q_OBJECT
public:
    Trainer(QVector<QSharedPointer<Pokemon>> team_, QSharedPointer<Bag> bag_);
    ~Trainer() = default;
    QVector<QSharedPointer<Pokemon>> getTeam();
    QSharedPointer<Bag> getBag();
    QString getName();

private:
    QVector<QSharedPointer<Pokemon>> team;
    QSharedPointer<Bag> bag;
    QString name;
};

#endif // TRAINER_H
