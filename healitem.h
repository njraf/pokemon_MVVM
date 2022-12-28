#ifndef HEALITEM_H
#define HEALITEM_H

#include <QObject>

#include "pokemon.h"

class HealItem : public QObject
{
    Q_OBJECT
public:
    explicit HealItem(QString name_, int healAmount_, int quantity_, QString description_, QObject *parent = nullptr);
    ~HealItem() = default;

    QString getName();
    int getHealAmount();
    int getQuantity();
    void setQuantity(int quantity_);
    QString getDescription();
    void use(QSharedPointer<Pokemon> pokemon);

private:
    QString name;
    int healAmount;
    int quantity;
    QString description;

signals:
    void usedLast();
};

#endif // HEALITEM_H
