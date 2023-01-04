#include "statusutilities.h"



QString statusToName(Status status) {
    switch (status) {
    case Status::NONE:
        return "";
    case Status::BURNED:
        return "BRN";
    case Status::FROZEN:
        return "FRZ";
    case Status::PARALYZED:
        return "PAR";
    case Status::POISONED:
        return "PSN";
    case Status::BADLY_POISONED:
        return "PSN";
    case Status::ASLEEP:
        return "SLP";
    default:
        return "";
    }
}

QString statusToColor(Status status) {
    switch (status) {
    case Status::NONE:
        return "lightgray";
    case Status::BURNED:
        return "red";
    case Status::FROZEN:
        return "cyan";
    case Status::PARALYZED:
        return "yellow";
    case Status::POISONED:
        return "purple";
    case Status::BADLY_POISONED:
        return "purple";
    case Status::ASLEEP:
        return "aquamarine";
    default:
        return "white";
    }
}
