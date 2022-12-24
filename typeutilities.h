#ifndef TYPEUTILITIES_H
#define TYPEUTILITIES_H

#include <QObject>

enum class Type : int {
    NONE, NORMAL, FIRE, WATER, ELECTRIC, GRASS, ICE, FIGHTING, POISON, GROUND, FLYING, PSYCHIC, BUG, ROCK, GHOST, DRAGON, DARK, STEEL, FAIRY
};

class TypeUtilities : public QObject
{
    Q_OBJECT
public:

    static double calcEffectiveness(Type attackType, Type defenseType);

    template <typename E>
    static constexpr typename std::underlying_type<E>::type to_underlying(E e) noexcept {
        return static_cast<typename std::underlying_type<E>::type>(e);
    }

private:
    explicit TypeUtilities(QObject *parent = nullptr);
    static double table[19][19];

signals:

};

#endif // TYPEUTILITIES_H
