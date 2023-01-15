#include "ownedpokemondao.h"
#include "natureutilities.h"

OwnedPokemonDao::OwnedPokemonDao(QObject *parent) : IDao(parent)
{
    db = QSqlDatabase::addDatabase(SQLITE_CONN, "OwnedPokemonDaoConnection");
    db.setHostName("MyHost");
    db.setDatabaseName("pokemon.db");
    db.setUserName("iamroot");
    db.setPassword("thisisme");
    if (!db.open()) {
        qDebug() << "ERROR: The OwnedPokemonDao database could not be opened." << db.lastError().text();
        qDebug() << "Valid database driver" << db.driverName() << db.isDriverAvailable(SQLITE_CONN);
        return;
    }

    populateDatabase();
}

void OwnedPokemonDao::insertPokemon(QSharedPointer<Pokemon> pokemon) {
    if (!db.isOpen()) {
        qDebug() << "OwnedPokemonDao database is not opened";
    }
    QSqlTableModel model(nullptr, db);
    model.setTable("OwnedPokemon");
    model.setEditStrategy(QSqlTableModel::OnRowChange);
    if (!model.select()) {
        qDebug() << "ERROR: OwnedPokemonDao::insertPokemon()::select() failed" << model.lastError().text();
    }

    QSqlRecord record = model.record();
    record.setValue("ID", pokemon->getID());
    record.setValue("NatDexNumber", pokemon->getDexNumber());
    record.setValue("Owner", pokemon->getOwner());
    record.setValue("NickName", ""); //TODO: make Pokemon::getNickname()
    record.setValue("CurrentHealth", pokemon->getHealthStat());
    record.setValue("Nature", NatureUtilities::to_underlying(pokemon->getNature()));
    record.setValue("Level", pokemon->getLevel());
    record.setValue("BoxNumber", pokemon->getBoxNumber());
    record.setValue("NumTurnsAsleep", pokemon->getStatusCondition()->getNumTurnsAsleep());
    record.setValue("MaxTurnsAsleep", pokemon->getStatusCondition()->getMaxSleepTurns());

    if (!model.insertRecord(-1, record)) {
        qDebug() << "ERROR: Last OwnedPokemonDao::insertRecord() error." << model.lastError().text();
        return;
    }

    if (!model.submitAll()) {
        qDebug() << "ERROR: Last OwnedPokemonDao::submitAll() error." << model.lastError().text();
        return;
    }
}

bool OwnedPokemonDao::populateDatabase() {
    // A new game does not start with any owned pokemon. remove the table and create an empty one.
    QSqlQuery query(db);
    if (!query.exec("DROP TABLE OwnedPokemon;")) {
        qDebug() << "Drop table failed" << query.lastError().text();
    }

    //TODO: add EVs and IVs
    if (!query.exec("CREATE TABLE OwnedPokemon("
                    "ID int PRIMARY KEY, "
                    "NatDexNumber int, "
                    "Owner varchar(32), "
                    "Nickname varchar(32), "
                    "CurrentHealth int, "
                    "Nature int, "
                    "Level int, "
                    "BoxNumber int, "
                    "NumTurnsAsleep int,"
                    "MaxTurnsAsleep int,"
                    "FOREIGN KEY (NatDexNumber) REFERENCES Pokemon(NatDexNumber));"))
    {
        qDebug() << "Create OwnedPokemon table failed" << query.lastError().text();
    }

    return true;
}
