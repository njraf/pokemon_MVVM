#include "attackmove.h"

QMap<QString, int> AttackMove::strToUnderlying = {
    {"PHYSICAL", to_underlying(Category::PHYSICAL)},
    {"SPECIAL", to_underlying(Category::SPECIAL)},
    {"STATUS", to_underlying(Category::STATUS)}
};

AttackMove::AttackMove(int id_, QString name_, int power_, int accuracy_, int pp_, int maxPP_, Type type_, Category category_,
                       std::function<void(QSharedPointer<Pokemon> self, QSharedPointer<Pokemon> opponent)> effect_, QObject *parent)
    : QObject(parent)
    , useEffect(effect_)
    , id(id_)
    , name(name_)
    , power(power_)
    , accuracy(accuracy_)
    , pp(pp_)
    , maxPP(maxPP_)
    , type(type_)
    , category(category_)
{

}

int AttackMove::getID() {
    return id;
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
