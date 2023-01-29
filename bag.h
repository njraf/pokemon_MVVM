#ifndef BAG_H
#define BAG_H

#include <QObject>
#include <QSharedPointer>
#include <QVector>

#include "healitem.h"
#include "pokeballitem.h"

class Bag : public QObject
{
    Q_OBJECT
public:
    explicit Bag(QObject *parent = nullptr);

    void addHealItem(QSharedPointer<HealItem> item);
    QVector<QSharedPointer<HealItem>> getHealItems();
    void addPokeballItem(QSharedPointer<PokeballItem> item);
    QVector<QSharedPointer<PokeballItem>> getPokeballItems();

private:
    QVector<QSharedPointer<HealItem>> healItems;
    QVector<QSharedPointer<PokeballItem>> pokeballItems;

signals:

};

#endif // BAG_H
