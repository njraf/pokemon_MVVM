#include "overworlddao.h"

OverworldDao::OverworldDao(QObject *parent) : IDao(parent)
{
    populateDatabase();
}

QVector<QVector<QSharedPointer<Tile>>> OverworldDao::getMapByID(int id) {
    if (id >= maps.size()) {
        qDebug() << "ERROR: Could not load the world. Map ID" << id << "does not match a file.";
        return QVector<QVector<QSharedPointer<Tile>>>();
    }

    QFile file(maps[id]);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "ERROR: Could not load the world. File" << maps[id] << "does not exist.";
        return QVector<QVector<QSharedPointer<Tile>>>();
    }

    // read file
    QTextStream in(&file);
    QString line = in.readLine();
    QVector<QVector<QSharedPointer<Tile>>> overworld;
    while (!line.isNull() && !line.isEmpty()) {
        QVector<QSharedPointer<Tile>> row;
        auto parts = line.split(',');
        for (int t = 0; t < parts.size(); t++) {
            int part = parts[t].simplified().toInt();
            for (auto type : Tile::tileTypes) {
                if (Tile::to_underlying(type) == part) {
                    row.append(QSharedPointer<Tile>::create(type));
                    break;
                }
            }
        }
        overworld.append(row);
        line = in.readLine();
    }

    // find max number of columns
    int maxCol = 0;
    for (auto row : overworld) {
        maxCol = std::max(static_cast<int>(row.size()), maxCol);
    }

    // append missing columns to rows
    // so all rows have the same number of columns
    for (auto &row : overworld) {
        if (row.size() < maxCol) {
            row.insert(row.size(), maxCol - row.size(), QSharedPointer<Tile>::create(TileType::NONE));
        }
    }

    return overworld;
}

bool OverworldDao::populateDatabase() {
    maps.append(":/zigzagpath.csv");
    return true;
}
