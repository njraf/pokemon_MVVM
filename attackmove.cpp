#include "attackmove.h"

AttackMove::AttackMove(QString name_, int power_, int pp_, int maxPP_, Type type_, Category category_, QObject *parent)
    : QObject(parent)
    , name(name_)
    , power(power_)
    , pp(pp_)
    , maxPP(maxPP_)
    , type(type_)
    , category(category_)
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

Category AttackMove::getCategory() const
{
    return category;
}
