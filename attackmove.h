#ifndef ATTACKMOVE_H
#define ATTACKMOVE_H

#include <QObject>

#include "typeutilities.h"

class AttackMove : public QObject
{
    Q_OBJECT
public:
    explicit AttackMove(QString name, int power, int pp, int maxPP, Type type_, QObject *parent = nullptr);
    ~AttackMove() = default;


    QString getName() const;
    int getPower() const;
    int getPp() const;
    void setPp(int newPp);
    int getMaxPP() const;
    Type getType() const;

private:
    QString name;
    int power;
    int pp;
    int maxPP;
    Type type;

signals:

};

#endif // ATTACKMOVE_H
