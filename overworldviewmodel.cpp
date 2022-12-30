#include "overworldviewmodel.h"

OverworldViewmodel::OverworldViewmodel(QObject *parent)
    : QObject(parent)
    , ROWS(21)
    , COLS(ROWS + (ROWS / 2))
    , playerRow(ROWS / 2)
    , playerCol(COLS / 2)
{
    for (int row = 0; row < ROWS; row++) {
        QVector<QSharedPointer<Tile>> r;
        for (int col = 0; col < COLS; col++) {
            if ((row % 2) == 0) {
                r.append(QSharedPointer<Tile>::create(TileType::GRASS));
            } else {
                r.append(QSharedPointer<Tile>::create(TileType::WALL));
            }
        }
        world.append(r);
    }
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

void OverworldViewmodel::move(QString direction) {
    if (direction == "up") {
        playerRow--;
    } else if (direction == "left") {
        playerCol--;
    } else if (direction == "down") {
        playerRow++;
    } else if (direction == "right") {
        playerCol++;
    }

    emit worldUpdated(world);
}
