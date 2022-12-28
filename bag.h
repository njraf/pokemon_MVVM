#ifndef BAG_H
#define BAG_H

#include <QObject>
#include <QSharedPointer>
#include <QVector>

#include "healitem.h"

class Bag : public QObject
{
    Q_OBJECT
public:
    explicit Bag(QObject *parent = nullptr);

    void addHealItem(QSharedPointer<HealItem> item);
    QVector<QSharedPointer<HealItem>> getHealItems();

private:
    QVector<QSharedPointer<HealItem>> healItems;

signals:

};

#endif // BAG_H
