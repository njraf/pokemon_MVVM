#ifndef ATTACKMOVE_H
#define ATTACKMOVE_H

#include <QObject>

#include "typeutilities.h"

enum class Category : int {
    PHYSICAL, SPECIAL, STATUS
};

class Pokemon;

class AttackMove : public QObject
{
    Q_OBJECT
public:
    explicit AttackMove(QString name_, int power_, int accuracy_, int pp_, int maxPP_, Type type_, Category category_,
                        std::function<void(QSharedPointer<Pokemon> self, QSharedPointer<Pokemon> opponent)> effect_, QObject *parent = nullptr);
    ~AttackMove() = default;


    QString getName() const;
    int getPower() const;
    int getAccuracy() const;
    int getPp() const;
    void setPp(int newPp);
    int getMaxPP() const;
    Type getType() const;
    Category getCategory() const;
    const std::function<void(QSharedPointer<Pokemon> self, QSharedPointer<Pokemon> opponent)> useEffect;

    template <typename E>
    static constexpr typename std::underlying_type<E>::type to_underlying(E e) noexcept {
        return static_cast<typename std::underlying_type<E>::type>(e);
    }

    static QMap<QString, int> strToUnderlying;

private:
    QString name;
    int power;
    int accuracy;
    int pp;
    int maxPP;
    Type type;
    Category category;

signals:

};

#endif // ATTACKMOVE_H
