#include "attackmovedao.h"

AttackMoveDao::AttackMoveDao(QObject *parent) : IDao(parent)
{
    db = QSqlDatabase::addDatabase(SQLITE_CONN, "AttackMoveDaoConnection");
    db.setHostName("MyHost");
    db.setDatabaseName("pokemon.db");
    db.setUserName("iamroot");
    db.setPassword("thisisme");
    if (!db.open()) {
        qDebug() << "ERROR: The AttackMoveDao database could not be opened." << db.lastError().text();
        qDebug() << "Valid database driver" << db.driverName() << db.isDriverAvailable(SQLITE_CONN);
        return;
    }

    populateDatabase();
}

QSharedPointer<AttackMove> AttackMoveDao::getAttackByID(int id) {
    if (!db.isOpen()) {
        qDebug() << "AttackMoveDao database is not opened";
    }
    QSqlTableModel model(nullptr, db);
    model.setTable("AttackMove");
    model.setFilter(QString("ID=%1").arg(id));
    if (!model.select()) {
        qDebug() << "ERROR: getPokemon()::select() failed" << db.lastError().text();
    }

    auto record = model.record(0);
    return QSharedPointer<AttackMove>::create(
                record.value("Name").toString(),
                record.value("Power").toInt(),
                record.value("Accuracy").toInt(),
                record.value("MaxPP").toInt(), // current PP
                record.value("MaxPP").toInt(), // max PP
                static_cast<Type>(record.value("Type").toInt()),
                static_cast<Category>(record.value("Category").toInt())
                );
}

bool AttackMoveDao::populateDatabase() {
    qDebug() << "Populating AttackMoves database";
    QSqlQuery query(db);
    if (!query.exec("DROP TABLE AttackMove;")) {
        qDebug() << "Drop table failed" << db.lastError().text();
    }
    if (!query.exec("CREATE TABLE AttackMove (ID int PRIMARY_KEY, Name varchar(32), Power int, Accuracy int, MaxPP int, Type int, Category int);")) {
        qDebug() << "Create table failed" << db.lastError().text();
    }

    int col = 0;
    QSqlTableModel table(nullptr, db);
    table.setTable("AttackMove");
    table.setEditStrategy(QSqlTableModel::OnManualSubmit);
    if (!table.select()) {
        qDebug() << "ERROR: First select() error." << db.lastError().text();
        return false;
    }

    table.setHeaderData(col++, Qt::Horizontal, "ID");
    table.setHeaderData(col++, Qt::Horizontal, "Name");
    table.setHeaderData(col++, Qt::Horizontal, "Power");
    table.setHeaderData(col++, Qt::Horizontal, "Accuracy");
    table.setHeaderData(col++, Qt::Horizontal, "MaxPP");
    table.setHeaderData(col++, Qt::Horizontal, "Type");
    table.setHeaderData(col++, Qt::Horizontal, "Category");
    if (!table.select()) {
        qDebug() << "ERROR: Second select() error." << db.lastError().text();
        return false;
    }

    // read from a csv file to populate the database
    QFile attackMoveFile("attackmoves.csv");
    if (!attackMoveFile.exists() || !attackMoveFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "ERROR:" << attackMoveFile.fileName() << "does not exist";
    } else {
        int row = table.rowCount();
        while (!attackMoveFile.atEnd()) {
            table.insertRow(row);
            QString line = attackMoveFile.readLine();
            QStringList parts = line.split(',');
            if (line.isEmpty() || (parts.count() != table.columnCount())) {
                continue;
            }

            for (int col = 0; col < parts.size(); col++) {
                QVariant data(parts[col].simplified());
                if ((col == 0) || ((col > 1) && (col < 5))) { // int data
                    if (!table.setData(table.index(row, col), data.toInt())) {
                        qDebug() << "ERROR: Could not set int data" << db.lastError().text();
                    }
                } else { // string data
                    QString strData = data.toString();
                    if (AttackMove::strToCategoryUnderlying.contains(strData)) {
                        int intData = AttackMove::strToCategoryUnderlying[strData];
                        if (!table.setData(table.index(row, col), intData)) {
                            qDebug() << "ERROR: Could not set category data" << db.lastError().text();
                        }
                    } else if (TypeUtilities::strToUnderlying.contains(strData)) {
                        int intData = TypeUtilities::strToUnderlying[strData];
                        if (!table.setData(table.index(row, col), intData)) {
                            qDebug() << "ERROR: Could not set type data" << db.lastError().text();
                        }
                    } else {
                        if (!table.setData(table.index(row, col), strData)) {
                            qDebug() << "ERROR: Could not set string data" << db.lastError().text();
                        }
                    }
                }
            }
            row++;
        }
    }
    attackMoveFile.close();

    if (!table.submitAll()) {
        qDebug() << "ERROR: Last submitAll() error." << db.lastError().text();
        return false;
    }

    return true;
}
