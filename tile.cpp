#include "tile.h"

Tile::Tile(TileType type_, QObject *parent)
    : QObject(parent)
    , type(type_)
{

}

TileType Tile::getType() {
    return type;
}
