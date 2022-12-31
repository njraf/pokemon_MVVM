#ifndef TILE_H
#define TILE_H

#include <QObject>

enum class TileType : int {
    NONE, DIRT, GRASS, TALL_GRASS, WALL
};

class Tile : public QObject
{
    Q_OBJECT
public:
    explicit Tile(TileType type_, QObject *parent = nullptr);
    ~Tile() = default;
    TileType getType();


    template <typename E>
    static constexpr typename std::underlying_type<E>::type to_underlying(E e) noexcept {
        return static_cast<typename std::underlying_type<E>::type>(e);
    }

private:
    TileType type;


signals:

};

#endif // TILE_H
