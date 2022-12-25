#ifndef NATUREUTILITIES_H
#define NATUREUTILITIES_H

#include <QObject>

// 1D array to 2D array[5][5]
// row = floor(index / 5) // integer division floors automatically
// col = index % 5

enum class Nature : int {
//- Attack Defense SpAttack SpDefense Speed
    HARDY, LONELY, ADAMANT, NAUGHTY, BRAVE, // + Attack
    BOLD, DOCILE, IMPISH, LAX, RELAXED,     // + Defense
    MODEST, MILD, BASHFUL, RASH, QUIET,     // + SpAttack
    CALM, GENTLE, CAREFUL, QUIRKY, SASSY,   // + SpDefense
    TIMID, HASTY, JOLLY, NAIVE, SERIOUS     // + Speed
};

class NatureUtilities : public QObject
{
    Q_OBJECT
public:
    enum class Stat : int {
        ATTACK, DEFENSE, SP_ATTACK, SP_DEFENSE, SPEED
    };

    static double calcMultiplier(Nature nature, NatureUtilities::Stat stat);
    static Nature randomNature();

    template <typename E>
    static constexpr typename std::underlying_type<E>::type to_underlying(E e) noexcept {
        return static_cast<typename std::underlying_type<E>::type>(e);
    }

private:
    explicit NatureUtilities(QObject *parent = nullptr);

signals:

};

#endif // NATUREUTILITIES_H
