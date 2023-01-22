#include "abilitydao.h"
#include "ability.h"

AbilityDao::AbilityDao(QObject *parent) : IDao(parent)
{
    db = QSqlDatabase::addDatabase(SQLITE_CONN, "AbilityDaoConnection");
    db.setHostName("MyHost");
    db.setDatabaseName("pokemon.db");
    db.setUserName("iamroot");
    db.setPassword("thisisme");
    if (!db.open()) {
        qDebug() << "ERROR: The AbilityDao database could not be opened." << db.lastError().text();
        qDebug() << "Valid database driver" << db.driverName() << db.isDriverAvailable(SQLITE_CONN);
        return;
    }

    populateDatabase();
}


bool AbilityDao::populateDatabase() {
    qDebug() << "Populating AbilityDao database";
    QSqlQuery query(db);
    if (!query.exec("DROP TABLE Abilities;")) {
        qDebug() << "Drop table failed" << query.lastError().text();
    }
    if (!query.exec("CREATE TABLE Abilities (ID int PRIMARY_KEY, Name varchar(32), Stage varchar(32) CHECK(Stage IN ('SUMMON', 'BEFORE_ATTACK', 'AFTER_ATTACK', 'BEFORE_OPPONENT_ATTACK', 'AFTER_OPPONENT_ATTACK', 'ATTACK_HITS', 'OPPONENT_HITS', 'START_TURN', 'END_TURN', 'FAINT''SUMMON', 'BEFORE_ATTACK', 'AFTER_ATTACK', 'BEFORE_OPPONENT_ATTACK', 'AFTER_OPPONENT_ATTACK', 'ATTACK_HITS', 'OPPONENT_HITS', 'START_TURN', 'END_TURN', 'FAINT')), Target varchar(8) CHECK(Target IN ('SELF', 'ALLY', 'OPPONENT')), Description varchar(128));")) {
        qDebug() << "Create table failed" << query.lastError().text();
    }

    int col = 0;
    QSqlTableModel table(nullptr, db);
    table.setTable("Abilities");
    table.setEditStrategy(QSqlTableModel::OnManualSubmit);
    if (!table.select()) {
        qDebug() << "ERROR: First select() error." << table.lastError().text();
        return false;
    }

    table.setHeaderData(col++, Qt::Horizontal, "ID");
    table.setHeaderData(col++, Qt::Horizontal, "Name");
    table.setHeaderData(col++, Qt::Horizontal, "Stage");
    table.setHeaderData(col++, Qt::Horizontal, "Target");
    table.setHeaderData(col++, Qt::Horizontal, "Description");
    if (!table.select()) {
        qDebug() << "ERROR: Second select() error." << table.lastError().text();
        return false;
    }

    // read from a csv file to populate the database
    QFile abilityFile("abilities.csv");
    if (!abilityFile.exists() || !abilityFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "ERROR:" << abilityFile.fileName() << "does not exist";
        return false;
    } else {
        int row = table.rowCount();
        while (!abilityFile.atEnd()) {
            table.insertRow(row);
            QString line = abilityFile.readLine();
            QStringList parts = line.split(',');
            if (line.isEmpty() || (parts.count() != table.columnCount())) {
                continue;
            }

            for (int col = 0; col < parts.size(); col++) {
                QVariant data(parts[col].simplified());
                if (col == 0) { // int data
                    if (!table.setData(table.index(row, col), data.toInt())) {
                        qDebug() << "ERROR: Could not set int data" << table.lastError().text();
                    }
                } else if (col == 1) {
                    if (!table.setData(table.index(row, col), data.toString())) {
                        qDebug() << "ERROR: Could not set Name data" << table.lastError().text();
                    }
                } else if (col == 4) {
                    if (!table.setData(table.index(row, col), data.toString())) {
                        qDebug() << "ERROR: Could not set Description data" << table.lastError().text();
                    }
                } else {
                    QString strData = data.toString();
                    if (Ability::strToBattleStage.keys().contains(strData)) {
                        if (!table.setData(table.index(row, col), strData)) {
                            qDebug() << "ERROR: Could not set BattleStage data" << table.lastError().text();
                        }
                    } else if (Ability::strToTarget.keys().contains(strData)) {
                        if (!table.setData(table.index(row, col), strData)) {
                            qDebug() << "ERROR: Could not set Target data" << table.lastError().text();
                        }
                    }
                }
            }
            row++;
        }
    }
    abilityFile.close();

    if (!table.submitAll()) {
        qDebug() << "ERROR: Last submitAll() error." << table.lastError().text();
        return false;
    }

    return true;
}
