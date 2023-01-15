#include "ownedpokemonattackmovedao.h"
#include "natureutilities.h"

#include <QVector>

OwnedPokemonAttackMoveDao::OwnedPokemonAttackMoveDao(QObject *parent) : IDao(parent)
{
    db = QSqlDatabase::addDatabase(SQLITE_CONN, "OwnedPokemonAttackMoveDaoConnection");
    db.setHostName("MyHost");
    db.setDatabaseName("pokemon.db");
    db.setUserName("iamroot");
    db.setPassword("thisisme");
    if (!db.open()) {
        qDebug() << "ERROR: The OwnedPokemonAttackMoveDao database could not be opened." << db.lastError().text();
        qDebug() << "Valid database driver" << db.driverName() << db.isDriverAvailable(SQLITE_CONN);
        return;
    }

    populateDatabase();
}

QSharedPointer<Pokemon> OwnedPokemonAttackMoveDao::getPokemonByID(int id) {
    if (!db.isOpen()) {
        qDebug() << "OwnedPokemonAttackMoveDao database is not opened";
    }
    QSqlQueryModel pokemonModel;
    pokemonModel.setQuery("SELECT * FROM OwnedPokemon INNER JOIN Pokemon ON OwnedPokemon.NatDexNumber=Pokemon.NatDexNumber WHERE ID=" + QString::number(id) + ";", db);

    QSqlQueryModel attackModel;
    attackModel.setQuery("SELECT * FROM OwnedPokemonAttackMove opam INNER JOIN AttackMove am ON opam.AttackMoveID = am.ID WHERE PokemonID=" + QString::number(id) + ";", db);

    QVector<QSharedPointer<AttackMove>> attackList;
    for (int r = 0; r < attackModel.rowCount(); r++) {
        auto record = attackModel.record(r);
        attackList.append(QSharedPointer<AttackMove>::create(
                              record.value("AttackMoveID").toInt(),
                              record.value("Name").toString(),
                              record.value("Power").toInt(),
                              record.value("Accuracy").toInt(),
                              record.value("CurrentPP").toInt(),
                              record.value("MaxPP").toInt(),
                              static_cast<Type>(record.value("Type").toInt()),
                              static_cast<Category>(record.value("Catgeory").toInt()),
                              [](QSharedPointer<Pokemon> self, QSharedPointer<Pokemon> opponent){} //TODO: save effect ID in database
                              ));
    }

    auto record = pokemonModel.record(0);
    auto asd = record.value("CurrentHealth").toInt();
    return QSharedPointer<Pokemon>::create(
                record.value("ID").toInt(),
                record.value("NatDexNumber").toInt(),
                record.value("Name").toString(), //TODO: use nickame if not empty
                record.value("Owner").toString(),
                record.value("BoxNumber").toInt(),
                static_cast<Nature>(record.value("Nature").toInt()),
                record.value("BaseHealth").toInt(),
                record.value("BaseAttack").toInt(),
                record.value("BaseDefense").toInt(),
                record.value("BaseSpAttack").toInt(),
                record.value("BaseSpDefense").toInt(),
                record.value("BaseSpeed").toInt(),
                record.value("CurrentHealth").toInt(),
                record.value("Level").toInt(),
                attackList,
                static_cast<Type>(record.value("Type1").toInt()),
                static_cast<Type>(record.value("Type2").toInt())
                );
}

void OwnedPokemonAttackMoveDao::insertPokemon(QSharedPointer<Pokemon> pokemon) {
    if (!db.isOpen()) {
        qDebug() << "OwnedPokemonAttackMoveDao database is not opened";
    }
    QSqlTableModel model(nullptr, db);
    model.setTable("OwnedPokemonAttackMove");
    model.setEditStrategy(QSqlTableModel::OnManualSubmit);
    if (!model.select()) {
        qDebug() << "ERROR: OwnedPokemonAttackMoveDao::insertPokemon()::select() failed" << db.lastError().text();
    }

    for (auto attack : pokemon->getAttackList()) {
        QSqlRecord record = model.record();
        record.setValue("PokemonID", pokemon->getID());
        record.setValue("AttackMoveID", attack->getID());
        record.setValue("CurrentPP", attack->getPp());

        if (!model.insertRecord(-1, record)) {
            qDebug() << "ERROR: Last OwnedPokemonAttackMoveDao::insertRecord() error." << model.lastError().text();
            return;
        }

        if (!model.submitAll()) {
            qDebug() << "ERROR: Last OwnedPokemonAttackMoveDao::submitAll() error." << model.lastError().text();
            return;
        }
    }
}

bool OwnedPokemonAttackMoveDao::populateDatabase() {
    // A new game does not start with any owned pokemon. remove the table and create an empty one.
    QSqlQuery query(db);
    if (!query.exec("DROP TABLE OwnedPokemonAttackMove;")) {
        qDebug() << "Drop table failed" << db.lastError().text();
    }

    if (!query.exec("CREATE TABLE OwnedPokemonAttackMove(PokemonID int, AttackMoveID int, CurrentPP int, PRIMARY KEY (PokemonID, AttackMoveID), FOREIGN KEY (PokemonID) REFERENCES OwnedPokemon(ID), FOREIGN KEY (AttackMoveID) REFERENCES AttackMove(ID));")) {
        qDebug() << "Create OwnedPokemonAttackMove table failed" << db.lastError().text();
    }

    return true;
}
