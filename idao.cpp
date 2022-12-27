#include "idao.h"

IDao::IDao(QObject *parent)
    : QObject(parent)
    , SQLITE_CONN("QSQLITE")
{

}

IDao::~IDao()
{
    if (db.open()) {
        db.close();
    }
}

