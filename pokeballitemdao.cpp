#include "pokeballitemdao.h"

PokeballItemDao::PokeballItemDao(QObject *parent) : IDao(parent)
{
    db = QSqlDatabase::addDatabase(SQLITE_CONN, "PokeballItemDaoConnection");
    db.setHostName("MyHost");
    db.setDatabaseName("pokemon.db");
    db.setUserName("iamroot");
    db.setPassword("thisisme");
    if (!db.open()) {
        qDebug() << "ERROR: The PokeballItemDao database could not be opened." << db.lastError().text();
        qDebug() << "Valid database driver" << db.driverName() << db.isDriverAvailable(SQLITE_CONN);
        return;
    }

    populateDatabase();
}

QSharedPointer<PokeballItem> PokeballItemDao::getPokeballItemByID(int id) {
    if (!db.isOpen()) {
        qDebug() << "PokeballItemDao database is not opened";
    }
    QSqlTableModel model(nullptr, db);
    model.setTable("PokeballItem");
    model.setFilter(QString("ID=%1").arg(id));
    if (!model.select()) {
        qDebug() << "ERROR: getPokeballItemByID()::select() failed" << db.lastError().text();
    }

    auto record = model.record(0);
    return QSharedPointer<PokeballItem>::create(
                record.value("Name").toString(),
                record.value("CatchRate").toDouble(),
                1
                );
}

bool PokeballItemDao::populateDatabase() {
    //qDebug() << "Populating PokeballItem database";
    QSqlQuery query(db);
    if (!query.exec("DROP TABLE PokeballItem;")) {
        qDebug() << "Drop table failed" << db.lastError().text();
    }
    if (!query.exec("CREATE TABLE PokeballItem(ID int PRIMARY_KEY, Name varchar(32), CatchRate double);")) {
        qDebug() << "Create table failed" << db.lastError().text();
    }

    int col = 0;
    QSqlTableModel table(nullptr, db);
    table.setTable("PokeballItem");
    table.setEditStrategy(QSqlTableModel::OnManualSubmit);
    if (!table.select()) {
        qDebug() << "ERROR: First select() error." << db.lastError().text();
        return false;
    }

    table.setHeaderData(col++, Qt::Horizontal, "ID");
    table.setHeaderData(col++, Qt::Horizontal, "Name");
    table.setHeaderData(col++, Qt::Horizontal, "CatchRate");
    if (!table.select()) {
        qDebug() << "ERROR: Second select() error." << db.lastError().text();
        return false;
    }

    // read from a csv file to populate the database
    QFile pokeballItemFile(":/pokeballitems.csv");
    if (!pokeballItemFile.exists() || !pokeballItemFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "ERROR:" << pokeballItemFile.fileName() << "does not exist";
    } else {
        int row = table.rowCount();
        while (!pokeballItemFile.atEnd()) {
            table.insertRow(row);
            QString line = pokeballItemFile.readLine();
            QStringList parts = line.split(',');
            if (line.isEmpty() || (parts.count() != table.columnCount())) {
                continue;
            }

            for (int col = 0; col < parts.size(); col++) {
                QVariant data(parts[col].simplified());
                if (col == 0) { // int data
                    if (!table.setData(table.index(row, col), data.toInt())) {
                        qDebug() << "ERROR: Could not set int data" << db.lastError().text();
                    }
                } else if (col == 1) { // string data
                    if (!table.setData(table.index(row, col), data.toString())) {
                        qDebug() << "ERROR: Could not set string data" << db.lastError().text();
                    }
                } else if (col == 2) { // double data
                    if (!table.setData(table.index(row, col), data.toString())) {
                        qDebug() << "ERROR: Could not set double data" << db.lastError().text();
                    }
                }
            }
            row++;
        }
    }
    pokeballItemFile.close();

    if (!table.submitAll()) {
        qDebug() << "ERROR: Last submitAll() error." << db.lastError().text();
        return false;
    }

    return true;
}
