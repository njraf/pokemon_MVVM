#include "attackmove.h"

QMap<QString, int> AttackMove::strToCategoryUnderlying = {
    {"NONE", to_underlying(Category::NONE)},
    {"PHYSICAL", to_underlying(Category::PHYSICAL)},
    {"SPECIAL", to_underlying(Category::SPECIAL)},
    {"OTHER", to_underlying(Category::OTHER)}
};

QMap<QString, Category> AttackMove::strToCategory = {
    {"NONE", Category::NONE},
    {"PHYSICAL", Category::PHYSICAL},
    {"SPECIAL", Category::SPECIAL},
    {"OTHER", Category::OTHER}
};

QMap<QString, Type> AttackMove::strToType = {
    {"NONE", Type::NONE},
    {"NORMAL", Type::NORMAL},
    {"FIRE", Type::FIRE},
    {"WATER", Type::WATER},
    {"ELECTRIC", Type::ELECTRIC},
    {"GRASS", Type::GRASS},
    {"ICE", Type::ICE},
    {"FIGHTING", Type::FIGHTING},
    {"POISON", Type::POISON},
    {"GROUND", Type::GROUND},
    {"FLYING", Type::FLYING},
    {"PSYCHIC", Type::PSYCHIC},
    {"BUG", Type::BUG},
    {"ROCK", Type::ROCK},
    {"GHOST", Type::GHOST},
    {"DRAGON", Type::DRAGON},
    {"DARK", Type::DARK},
    {"STEEL", Type::STEEL},
    {"FAIRY", Type::FAIRY}
};

AttackMove::AttackMove(QString name_, int power_, int accuracy_, int pp_, int maxPP_, Type type_, Category category_, QObject *parent)
    : QObject(parent)
    , name(name_)
    , power(power_)
    , accuracy(accuracy_)
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

int AttackMove::getAccuracy() const
{
    return accuracy;
}

int AttackMove::getPp() const
{
    return pp;
}

void AttackMove::setPp(int newPp)
{
    pp = newPp;
    if (pp < 0) {
        pp = 0;
    } else if (pp > maxPP) {
        pp = maxPP;
    }
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
