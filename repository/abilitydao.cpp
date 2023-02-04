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

Ability AbilityDao::getAbilityByID(int id) {
    if (!db.isOpen()) {
        qDebug() << "AbilityDao database is not opened";
    }
    QSqlTableModel model(nullptr, db);
    model.setTable("Abilities");
    model.setFilter(QString("ID=%1").arg(id));
    if (!model.select()) {
        qDebug() << "ERROR: getPokemon()::select() failed" << model.lastError().text();
    }

    auto record = model.record(0);
    return Ability(
                record.value("ID").toInt(),
                record.value("Name").toString(),
                {Ability::strToBattleStage[record.value("Stage").toString()]},
                Ability::strToTarget[record.value("Target").toString()],
                AttackMove::strToCategory[record.value("AttackCategory").toString()],
                AttackMove::strToType[record.value("AttackType").toString()],
                //record.value("Description").toString(),
                AbilityFactory::getAbility(record.value("ID").toInt())
                );
}

bool AbilityDao::populateDatabase() {
    //qDebug() << "Populating AbilityDao database";
    QSqlQuery query(db);
    if (!query.exec("DROP TABLE Abilities;")) {
        qDebug() << "Drop table failed" << query.lastError().text();
    }
    if (!query.exec("CREATE TABLE Abilities (ID int PRIMARY_KEY, "
                    "Name varchar(32), "
                    "Stage varchar(32) CHECK(Stage IN ('SWITCH_IN', 'SWITCH_OUT', 'BEFORE_ATTACK', 'AFTER_ATTACK', 'BEFORE_OPPONENT_ATTACK', 'AFTER_OPPONENT_ATTACK', 'ATTACK_HITS', 'OPPONENT_HITS', 'START_TURN', 'END_TURN', 'FAINT', 'OPPONENT_FAINT', 'STATUS_APPLIED', 'STATUS_REMOVED', 'FLINCH')), "
                    "Target varchar(8) CHECK(Target IN ('SELF', 'ALLY', 'OPPONENT')), "
                    "AttackCategory varchar(16) CHECK(AttackCategory IN ('NONE', 'SPECIAL', 'PHYSICAL', 'STATUS')), "
                    "AttackType varchar(16) CHECK(AttackType IN ('NONE', 'NORMAL', 'FIRE', 'WATER', 'ELECTRIC', 'GRASS', 'ICE', 'FIGHTING', 'POISON', 'GROUND', 'FLYING', 'PSYCHIC', 'BUG', 'ROCK', 'GHOST', 'DRAGON', 'DARK', 'STEEL', 'FAIRY')), "
                    "Description varchar(128));")) {
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
    table.setHeaderData(col++, Qt::Horizontal, "AttackCategory");
    table.setHeaderData(col++, Qt::Horizontal, "AttackType");
    table.setHeaderData(col++, Qt::Horizontal, "Description");
    if (!table.select()) {
        qDebug() << "ERROR: Second select() error." << table.lastError().text();
        return false;
    }

    // read from a csv file to populate the database
    QFile abilityFile(":/abilities.csv");
    if (!abilityFile.exists() || !abilityFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "ERROR:" << abilityFile.fileName() << "does not exist";
        return false;
    } else {
        int row = table.rowCount();
        while (!abilityFile.atEnd()) {
            table.insertRow(row);
            QString line = abilityFile.readLine();
            QStringList parts = line.split(',');

            const int NUM_EXPECTED_FIELDS = 7;
            if (parts.size() > NUM_EXPECTED_FIELDS) { // expected number of fields
                // description likely has commas
                QString description = "";
                for (int i = NUM_EXPECTED_FIELDS - 1; i < parts.size();) {
                    description += parts.takeAt(i);
                    if (i < NUM_EXPECTED_FIELDS) {
                        description += ", ";
                    }
                }
                parts.append(description);
            }

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
                } else if (col == 6) {
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
                    } else if (AttackMove::strToCategory.keys().contains(strData)) {
                        if (!table.setData(table.index(row, col), strData)) {
                            qDebug() << "ERROR: Could not set AttackCategory data" << table.lastError().text();
                        }
                    } else if (AttackMove::strToType.keys().contains(strData)) {
                        if (!table.setData(table.index(row, col), strData)) {
                            qDebug() << "ERROR: Could not set AttackType data" << table.lastError().text();
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
