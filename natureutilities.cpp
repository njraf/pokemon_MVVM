#include "natureutilities.h"

#include <QDebug>
#include <QtMath>
#include <QRandomGenerator>

NatureUtilities::NatureUtilities(QObject *parent) : QObject(parent)
{

}

double NatureUtilities::calcMultiplier(Nature nature, NatureUtilities::Stat stat) {
    const int row = to_underlying(nature) / 5;
    const int col = to_underlying(nature) % 5;
    const int statIndex = to_underlying(stat);
    double multiplier = 1.0;

    if (row == statIndex) {
        multiplier *= 1.1;
    }

    if (col == statIndex) {
        multiplier *= 0.9;
    }

    const qreal tolerance = 0.09999999;
    const qreal value = qFabs(1.0 - multiplier);
    if (value < tolerance) {
        multiplier = 1.0;
    }

    QMap<Nature, QString> natmap = {
        {Nature::HARDY, "HARDY"},
        {Nature::LONELY, "LONELY"},
        {Nature::ADAMANT, "ADAMANT"},
        {Nature::NAUGHTY, "NAUGHTY"},
        {Nature::BRAVE, "BRAVE"},
        {Nature::BOLD, "BOLD"},
        {Nature::DOCILE, "DOCILE"},
        {Nature::IMPISH, "IMPISH"},
        {Nature::LAX, "LAX"},
        {Nature::RELAXED, "RELAXED"},
        {Nature::MODEST, "MODEST"},
        {Nature::MILD, "MILD"},
        {Nature::BASHFUL, "BASHFUL"},
        {Nature::RASH, "RASH"},
        {Nature::QUIET, "QUIET"},
        {Nature::CALM, "CALM"},
        {Nature::GENTLE, "GENTLE"},
        {Nature::CAREFUL, "CAREFUL"},
        {Nature::QUIRKY, "QUIRKY"},
        {Nature::SASSY, "SASSY"},
        {Nature::TIMID, "TIMID"},
        {Nature::HASTY, "HASTY"},
        {Nature::JOLLY, "JOLLY"},
        {Nature::NAIVE, "NAIVE"},
        {Nature::SERIOUS, "SEIOUS"},
    };

    QMap<NatureUtilities::Stat, QString> statmap = {
        {NatureUtilities::Stat::ATTACK, "ATTACK"},
        {NatureUtilities::Stat::DEFENSE, "DEFENSE"},
        {NatureUtilities::Stat::SP_ATTACK, "SP_ATTACK"},
        {NatureUtilities::Stat::SP_DEFENSE, "SP_DEFENCE"},
        {NatureUtilities::Stat::SPEED, "SPEED"},
    };

    //qDebug() << "Nature multiplier" << multiplier << "for nature" << natmap[nature] << "and stat" << statmap[stat];
    return multiplier;
}

Nature NatureUtilities::randomNature() {
    return static_cast<Nature>(QRandomGenerator::global()->generate() % 25);
}
