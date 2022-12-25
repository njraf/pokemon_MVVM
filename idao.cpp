#include "idao.h"

IDao::IDao(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE", "IDaoConnection");
    db.setHostName("MyHost");
    db.setDatabaseName("pokemon.db");
    db.setUserName("iamroot");
    db.setPassword("thisisme");
    if (!db.open()) {
        qDebug() << "ERROR: The IDao database could not be opened." << db.lastError().text();
        qDebug() << "Valid database driver" << db.driverName() << db.isDriverAvailable("QSQLITE");
        return;
    }
}

IDao::~IDao()
{
    if (db.open()) {
        db.close();
    }
}

