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

    // make the overworld
    world = repository->getMapByID(0);
    int maxCol = 0;
    for (auto row : world) {
        maxCol = std::max(row.size(), maxCol);
    }
    ROWS = world.size();
    COLS = maxCol;
    playerRow = (ROWS / 2);
    playerCol = (COLS / 2);
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
    if (paused || world.isEmpty() || world[0].isEmpty()) {
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

        auto wildPokemon = repository->getPokemon(1);
        wildPokemon->setAttackList(attackList);

        QVector<QSharedPointer<Pokemon>> opponentTeam = {wildPokemon};
        QSharedPointer<Trainer> opponent = QSharedPointer<Trainer>::create(QSharedPointer<QVector<QSharedPointer<Pokemon>>>::create(opponentTeam), QSharedPointer<Bag>::create());

        QVector<QVariant> data = {QVariant::fromValue<QSharedPointer<Trainer>>(opponent)};
        PageNavigator::getInstance()->navigate(PageName::BATTLE, data);
    }
}

void OverworldViewmodel::togglePause() {
    paused = !paused;
}
