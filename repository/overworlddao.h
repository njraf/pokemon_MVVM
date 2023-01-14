#ifndef OVERWORLDDAO_H
#define OVERWORLDDAO_H

#include <QObject>
#include <QVector>
#include <QSharedPointer>

#include "tile.h"
#include "idao.h"

class OverworldDao : public IDao
{
    Q_OBJECT
public:
    explicit OverworldDao(QObject *parent = nullptr);
    ~OverworldDao() = default;
    QVector<QVector<QSharedPointer<Tile>>> getMapByID(int id);

protected:
    bool populateDatabase() override;

private:
    QVector<QString> maps;

};

#endif // OVERWORLDDAO_H
