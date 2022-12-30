#ifndef TILE_H
#define TILE_H

#include <QObject>

enum class TileType : int {
    NONE, DIRT, GRASS, WALL
};

class Tile : public QObject
{
    Q_OBJECT
public:
    explicit Tile(TileType type_, QObject *parent = nullptr);
    ~Tile() = default;
    TileType getType();


private:
    TileType type;


signals:

};

#endif // TILE_H
