#include "healitemdao.h"

HealItemDao::HealItemDao(QObject *parent) : IDao(parent)
{
    db = QSqlDatabase::addDatabase(SQLITE_CONN, "HealItemDaoConnection");
    db.setHostName("MyHost");
    db.setDatabaseName("pokemon.db");
    db.setUserName("iamroot");
    db.setPassword("thisisme");
    if (!db.open()) {
        qDebug() << "ERROR: The HealItemDao database could not be opened." << db.lastError().text();
        qDebug() << "Valid database driver" << db.driverName() << db.isDriverAvailable(SQLITE_CONN);
        return;
    }

    populateDatabase();
}

QSharedPointer<HealItem> HealItemDao::getHealItemByID(int id) {
    if (!db.isOpen()) {
        qDebug() << "HealItemDao database is not opened";
    }
    QSqlTableModel model(nullptr, db);
    model.setTable("HealItem");
    model.setFilter(QString("ID=%1").arg(id));
    if (!model.select()) {
        qDebug() << "ERROR: getHealItemByID()::select() failed" << db.lastError().text();
    }

    auto record = model.record(0);
    return QSharedPointer<HealItem>::create(
                record.value("Name").toString(),
                record.value("HP").toInt(),
                1,
                record.value("Description").toString()
                );
}

bool HealItemDao::populateDatabase() {
    //qDebug() << "Populating HealItem database";
    QSqlQuery query(db);
    if (!query.exec("DROP TABLE HealItem;")) {
        qDebug() << "Drop table failed" << db.lastError().text();
    }
    if (!query.exec("CREATE TABLE HealItem(ID int PRIMARY_KEY, Name varchar(32), HP int, Description varchar(128));")) {
        qDebug() << "Create table failed" << db.lastError().text();
    }

    int col = 0;
    QSqlTableModel table(nullptr, db);
    table.setTable("HealItem");
    table.setEditStrategy(QSqlTableModel::OnManualSubmit);
    if (!table.select()) {
        qDebug() << "ERROR: First select() error." << db.lastError().text();
        return false;
    }

    table.setHeaderData(col++, Qt::Horizontal, "ID");
    table.setHeaderData(col++, Qt::Horizontal, "Name");
    table.setHeaderData(col++, Qt::Horizontal, "HP");
    table.setHeaderData(col++, Qt::Horizontal, "Description");
    if (!table.select()) {
        qDebug() << "ERROR: Second select() error." << db.lastError().text();
        return false;
    }

    // read from a csv file to populate the database
    QFile healItemFile("repository/healitems.csv");
    if (!healItemFile.exists() || !healItemFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "ERROR:" << healItemFile.fileName() << "does not exist";
    } else {
        int row = table.rowCount();
        while (!healItemFile.atEnd()) {
            table.insertRow(row);
            QString line = healItemFile.readLine();
            QStringList parts = line.split(',');
            if (line.isEmpty() || (parts.count() != table.columnCount())) {
                continue;
            }

            for (int col = 0; col < parts.size(); col++) {
                QVariant data(parts[col].simplified());
                if (col == 2) { // int data
                    if (!table.setData(table.index(row, col), data.toInt())) {
                        qDebug() << "ERROR: Could not set int data" << db.lastError().text();
                    }
                } else { // string data
                    if (!table.setData(table.index(row, col), data.toString())) {
                        qDebug() << "ERROR: Could not set string data" << db.lastError().text();
                    }
                }
            }
            row++;
        }
    }
    healItemFile.close();

    if (!table.submitAll()) {
        qDebug() << "ERROR: Last submitAll() error." << db.lastError().text();
        return false;
    }

    return true;
}
