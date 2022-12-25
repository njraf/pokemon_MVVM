#ifndef ATTACKMOVE_H
#define ATTACKMOVE_H

#include <QObject>

#include "typeutilities.h"

enum class Category : int {
    PHYSICAL, SPECIAL, OTHER
};

class AttackMove : public QObject
{
    Q_OBJECT
public:
    explicit AttackMove(QString name_, int power_, int accuracy_, int pp_, int maxPP_, Type type_, Category category_, QObject *parent = nullptr);
    ~AttackMove() = default;


    QString getName() const;
    int getPower() const;
    int getAccuracy() const;
    int getPp() const;
    void setPp(int newPp);
    int getMaxPP() const;
    Type getType() const;
    Category getCategory() const;

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
