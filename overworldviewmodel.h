#ifndef OVERWORLDVIEWMODEL_H
#define OVERWORLDVIEWMODEL_H

#include <QObject>
#include <QSharedPointer>
#include <QVector>

#include "tile.h"

class OverworldViewmodel : public QObject
{
    Q_OBJECT
public:
    explicit OverworldViewmodel(QObject *parent = nullptr);
    ~OverworldViewmodel() = default;
    QVector<QVector<QSharedPointer<Tile>>> getWorld();
    int getPlayerRow();
    int getPlayerCol();
    void move(QString direction);

private:
    const int ROWS;
    const int COLS;
    int playerRow;
    int playerCol;
    QVector<QVector<QSharedPointer<Tile>>> world;

signals:
    void worldUpdated(QVector<QVector<QSharedPointer<Tile>>> world);

};

#endif // OVERWORLDVIEWMODEL_H
