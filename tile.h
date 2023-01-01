#ifndef TILE_H
#define TILE_H

#include <QObject>

enum class TileType : int {
    NONE, // default
    TALL_GRASS, // special tiles
    SHALLOW_WATER, WATER, DEEP_WATER, // water
    DIRT_PATH, GRASS_PATH, ROCK_PATH, STONE_PATH, SAND_PATH, // passable tiles (lighter colors)
    ROCK_WALL, STONE_WALL // impassable tiles (darker colors)
};

class Tile : public QObject
{
    Q_OBJECT
public:
    explicit Tile(TileType type_, QObject *parent = nullptr);
    ~Tile() = default;
    TileType getType();
    bool getPassable();


    template <typename E>
    static constexpr typename std::underlying_type<E>::type to_underlying(E e) noexcept {
        return static_cast<typename std::underlying_type<E>::type>(e);
    }

    static const QVector<TileType> tileTypes;

private:
    TileType type;
    bool passable; // player can step on this tile

};

#endif // TILE_H
