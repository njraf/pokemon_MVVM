#include "tile.h"

const QVector<TileType> Tile::tileTypes = {
        TileType::NONE, // default
        TileType::TALL_GRASS, // special tiles
        TileType::SHALLOW_WATER, TileType::WATER, TileType::DEEP_WATER, // water
        TileType::DIRT_PATH, TileType::GRASS_PATH, TileType::ROCK_PATH, TileType::STONE_PATH, TileType::SAND_PATH, // passable tiles (lighter colors)
        TileType::ROCK_WALL, TileType::STONE_WALL // impassable tiles (darker colors)
    };

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
