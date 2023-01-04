#ifndef STATUSUTILITIES_H
#define STATUSUTILITIES_H

#include <QString>

enum class Status : int {
    NONE, BURNED, FROZEN, PARALYZED, POISONED, BADLY_POISONED, ASLEEP
};

QString statusToName(Status status);
QString statusToColor(Status status);

#endif // STATUSUTILITIES_H
