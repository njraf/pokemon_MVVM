#include "ownedpokemondao.h"

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

QSharedPointer<Pokemon> OwnedPokemonDao::getPokemonByID(int id) {
    if (!db.isOpen()) {
        qDebug() << "OwnedPokemonDao database is not opened";
    }
    QSqlQueryModel model;
    model.setQuery("SELECT * FROM ((OwnedPokemon INNER JOIN Pokemon ON OwnedPokemon.PokemonID=Pokemon.NatDexNumber) INNER JOIN AttackMove ON OwnedPokemon.AttackMoveID=AttackMove.ID) WHERE PokemonID=" + QString::number(id) + ";", db);

    for (int i = 0; i < model.rowCount(); i++) {
        auto record = model.record(i);
        for (int ii = 0; ii < record.count(); ii++) {
            qDebug() << record.value(ii);
        }
    }

    return QSharedPointer<Pokemon>(); //TODO: test after inserting data
}

bool OwnedPokemonDao::populateDatabase() {
    // A new game does not start with any owned pokemon. remove the table and create an empty one.
    QSqlQuery query(db);
    if (!query.exec("DROP TABLE OwnedPokemon;")) {
        qDebug() << "Drop table failed" << db.lastError().text();
    }

    if (!query.exec("CREATE TABLE OwnedPokemon(PokemonID int, AttackMoveID int, CurrentPP int, BoxNumber int, PRIMARY KEY (PokemonID, AttackMoveID), FOREIGN KEY (PokemonID) REFERENCES Pokemon(NatDexNumber), FOREIGN KEY (AttackMoveID) REFERENCES AttackMove(ID));")) {
        qDebug() << "Create table failed" << db.lastError().text();
    }

    return true;
}
