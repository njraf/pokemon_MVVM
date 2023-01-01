#include "tile.h"

Tile::Tile(TileType type_, QObject *parent)
    : QObject(parent)
    , type(type_)
    , passable(true)
{
    switch (type) {
    case TileType::NONE:
    case TileType::ROCK_WALL:
    case TileType::STONE_WALL:
        passable = false;
        break;
    default:
        break;
    }
}

TileType Tile::getType() {
    return type;
}

bool Tile::getPassable() {
    return passable;
}
