#ifndef BOXVIEWMODEL_H
#define BOXVIEWMODEL_H

#include <QObject>
#include <QSharedPointer>

#include "repository.h"
#include "pokemon.h"

class BoxViewmodel : public QObject
{
    Q_OBJECT
public:
    explicit BoxViewmodel(QSharedPointer<Repository> repository_, QObject *parent = nullptr);
    ~BoxViewmodel() = default;

    const int ROWS = 5;
    const int COLS = 5;

private:
    QSharedPointer<Repository> repository;

signals:

};

#endif // BOXVIEWMODEL_H
