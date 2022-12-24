#include "attackmove.h"

AttackMove::AttackMove(QString name_, int power_, int pp_, int maxPP_, Type type_, QObject *parent)
    : QObject(parent)
    , name(name_)
    , power(power_)
    , pp(pp_)
    , maxPP(maxPP_)
    , type(type_)
{

}

QString AttackMove::getName() const
{
    return name;
}

int AttackMove::getPower() const
{
    return power;
}

int AttackMove::getPp() const
{
    return pp;
}

void AttackMove::setPp(int newPp)
{
    pp = newPp;
}

int AttackMove::getMaxPP() const
{
    return maxPP;
}

Type AttackMove::getType() const
{
    return type;
}
