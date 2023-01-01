#include "overworldviewmodel.h"
#include "pagenavigator.h"
#include "bag.h"
#include "trainer.h"

OverworldViewmodel::OverworldViewmodel(QSharedPointer<Repository> repository_, QObject *parent)
    : QObject(parent)
    , ROWS(21)
    , COLS(ROWS + (ROWS / 2))
    , playerRow(ROWS / 2)
    , playerCol(COLS / 2)
    , repository(repository_)
    , paused(false)
{
    for (int row = 0; row < ROWS; row++) {
        QVector<QSharedPointer<Tile>> r;
        for (int col = 0; col < COLS; col++) {
            if ((col % 5) == 3) {
                r.append(QSharedPointer<Tile>::create(TileType::ROCK_PATH));
            } else if (row == (playerRow - 1)) {
                r.append(QSharedPointer<Tile>::create(TileType::TALL_GRASS));
            } else if ((row % 2) == 0) {
                r.append(QSharedPointer<Tile>::create(TileType::GRASS_PATH));
            } else {
                r.append(QSharedPointer<Tile>::create(TileType::STONE_WALL));
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
    if (paused) {
        return;
    }

    if (direction == "up") {
        if ((playerRow == 0) || (!world[playerRow - 1][playerCol]->getPassable())) {
            return;
        }
        playerRow--;
    } else if (direction == "left") {
        if ((playerCol == 0) || (!world[playerRow][playerCol - 1]->getPassable())) {
            return;
        }
        playerCol--;
    } else if (direction == "down") {
        if ((playerRow == (ROWS - 1)) || (!world[playerRow + 1][playerCol]->getPassable())) {
            return;
        }
        playerRow++;
    } else if (direction == "right") {
        if ((playerCol == (COLS - 1)) || (!world[playerRow][playerCol + 1]->getPassable())) {
            return;
        }
        playerCol++;
    }

    emit worldUpdated(world);

    // 10% chance to encounter a wild pokemon in tall grass
    if ((world[playerRow][playerCol]->getType() == TileType::TALL_GRASS) && ((QRandomGenerator::global()->generate() % 10) == 0)) {
        QVector<QSharedPointer<AttackMove>> attackList;
        attackList.append(repository->getAttackByID(2));

        auto wildPokemon = repository->getPokemon(4);
        wildPokemon->setAttackList(attackList);

        QVector<QSharedPointer<Pokemon>> opponentTeam = {wildPokemon};
        QSharedPointer<Trainer> opponent = QSharedPointer<Trainer>::create(opponentTeam, QSharedPointer<Bag>::create());

        QVector<QVariant> data = {QVariant::fromValue<QSharedPointer<Trainer>>(opponent)};
        PageNavigator::getInstance()->navigate(PageName::BATTLE, data);
    }
}

void OverworldViewmodel::togglePause() {
    paused = !paused;
}
