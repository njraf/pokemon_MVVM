#include "overworldviewmodel.h"
#include "pagenavigator.h"

OverworldViewmodel::OverworldViewmodel(QSharedPointer<Repository> repository_, QObject *parent)
    : QObject(parent)
    , ROWS(21)
    , COLS(ROWS + (ROWS / 2))
    , playerRow(ROWS / 2)
    , playerCol(COLS / 2)
    , repository(repository_)
{
    for (int row = 0; row < ROWS; row++) {
        QVector<QSharedPointer<Tile>> r;
        for (int col = 0; col < COLS; col++) {
            if (row == (playerRow - 1)) {
                r.append(QSharedPointer<Tile>::create(TileType::TALL_GRASS));
            } else if ((row % 2) == 0) {
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
        if (playerRow == 0) {
            return;
        }
        playerRow--;
    } else if (direction == "left") {
        if (playerCol == 0) {
            return;
        }
        playerCol--;
    } else if (direction == "down") {
        if (playerRow == (ROWS - 1)) {
            return;
        }
        playerRow++;
    } else if (direction == "right") {
        if (playerCol == (COLS - 1)) {
            return;
        }
        playerCol++;
    }

    emit worldUpdated(world);

    if (world[playerRow][playerCol]->getType() == TileType::TALL_GRASS) {
        auto wildPokemon = repository->getPokemon(4);
        QVector<QVariant> data = {QVariant::fromValue<QSharedPointer<Pokemon>>(wildPokemon)};
        PageNavigator::getInstance()->navigate(PageName::BATTLE);
    }
}
