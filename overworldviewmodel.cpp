#include "overworldviewmodel.h"

OverworldViewmodel::OverworldViewmodel(QObject *parent)
    : QObject(parent)
    , ROWS(11)
    , COLS(ROWS + (ROWS / 2))
    , playerRow(ROWS / 2)
    , playerCol(COLS / 2)
{

}

QVector<QVector<QSharedPointer<Tile>>> OverworldViewmodel::getWorld() {
    return world;
}

int OverworldViewmodel::getPlayerRow() {
    return playerRow;
}

int OverworldViewmodel::getPlayerCol() {
    return playerCol;
}
