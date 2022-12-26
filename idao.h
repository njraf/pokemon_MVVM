#ifndef IDAO_H
#define IDAO_H

#include <QObject>
#include <QtSql>

class IDao : public QObject
{
    Q_OBJECT
public:
    explicit IDao(QObject *parent = nullptr);
    virtual ~IDao();

protected:
    QSqlDatabase db;
    const QString SQLITE_CONN;

signals:

};

#endif // IDAO_H
