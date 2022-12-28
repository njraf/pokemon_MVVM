#ifndef BAGVIEWMODEL_H
#define BAGVIEWMODEL_H

#include <QObject>

#include "bag.h"
#include "healitem.h"

class BagViewmodel : public QObject
{
    Q_OBJECT
public:
    explicit BagViewmodel(QSharedPointer<Bag> bag_, QObject *parent = nullptr);
    ~BagViewmodel() = default;

    void addHealItem(QSharedPointer<HealItem> item);
    QVector<QSharedPointer<HealItem>> getHealItems();

private:
    QSharedPointer<Bag> bag;

signals:

};

#endif // BAGVIEWMODEL_H
