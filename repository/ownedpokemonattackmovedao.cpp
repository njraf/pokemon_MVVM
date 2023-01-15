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

    QVector<QSharedPointer<AttackMove>> attackList = populateAttackList(attackModel);

    return makePokemon(pokemonModel.record(0), attackList);
}

void OwnedPokemonAttackMoveDao::insertPokemon(QSharedPointer<Pokemon> pokemon) {
    if (!db.isOpen()) {
        qDebug() << "OwnedPokemonAttackMoveDao database is not opened";
    }
    QSqlTableModel model(nullptr, db);
    model.setTable("OwnedPokemonAttackMove");
    model.setEditStrategy(QSqlTableModel::OnManualSubmit);
    if (!model.select()) {
        qDebug() << "ERROR: OwnedPokemonAttackMoveDao::insertPokemon()::select() failed" << model.lastError().text();
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

QVector<QSharedPointer<Pokemon>> OwnedPokemonAttackMoveDao::getPartyPokemon() {
    if (!db.isOpen()) {
        qDebug() << "OwnedPokemonAttackMoveDao database is not opened";
    }
    QSqlQueryModel pokemonModel;
    pokemonModel.setQuery("SELECT * FROM OwnedPokemon INNER JOIN Pokemon ON OwnedPokemon.NatDexNumber=Pokemon.NatDexNumber WHERE BoxNumber=0;", db);

    QVector<QSharedPointer<Pokemon>> party;
    for (int i = 0; i < pokemonModel.rowCount(); i++) {
        QSqlQueryModel attackModel;
        attackModel.setQuery("SELECT * FROM OwnedPokemonAttackMove opam INNER JOIN AttackMove am ON opam.AttackMoveID = am.ID INNER JOIN OwnedPokemon op ON op.ID = opam.PokemonID WHERE BoxNumber=0 AND PokemonID=" + QString::number(pokemonModel.record(i).value("ID").toInt()) + ";", db);

        QVector<QSharedPointer<AttackMove>> attackList = populateAttackList(attackModel);

        party.append(makePokemon(pokemonModel.record(i), attackList));

    }

    return party;
}

QVector<QSharedPointer<Pokemon>> OwnedPokemonAttackMoveDao::getAllPokemonFromBox(int box) {
    if (!db.isOpen()) {
        qDebug() << "OwnedPokemonAttackMoveDao database is not opened";
    }
    QSqlQueryModel pokemonModel;
    pokemonModel.setQuery("SELECT * FROM OwnedPokemon INNER JOIN Pokemon ON OwnedPokemon.NatDexNumber=Pokemon.NatDexNumber WHERE BoxNumber=" + QString::number(box) + ";", db);

    QVector<QSharedPointer<Pokemon>> party;
    for (int i = 0; i < pokemonModel.rowCount(); i++) {
        QSqlQueryModel attackModel;
        attackModel.setQuery("SELECT * FROM OwnedPokemonAttackMove opam INNER JOIN AttackMove am ON opam.AttackMoveID = am.ID INNER JOIN OwnedPokemon op ON op.ID = opam.PokemonID WHERE BoxNumber=" + QString::number(box) + " AND PokemonID=" + QString::number(pokemonModel.record(i).value("ID").toInt()) + ";", db);

        QVector<QSharedPointer<AttackMove>> attackList = populateAttackList(attackModel);

        party.append(makePokemon(pokemonModel.record(i), attackList));

    }

    return party;
}

bool OwnedPokemonAttackMoveDao::populateDatabase() {
    // A new game does not start with any owned pokemon. remove the table and create an empty one.
    QSqlQuery query(db);
    if (!query.exec("DROP TABLE OwnedPokemonAttackMove;")) {
        qDebug() << "Drop table failed" << query.lastError().text();
    }

    if (!query.exec("CREATE TABLE OwnedPokemonAttackMove(PokemonID int, AttackMoveID int, CurrentPP int, PRIMARY KEY (PokemonID, AttackMoveID), FOREIGN KEY (PokemonID) REFERENCES OwnedPokemon(ID), FOREIGN KEY (AttackMoveID) REFERENCES AttackMove(ID));")) {
        qDebug() << "Create OwnedPokemonAttackMove table failed" << query.lastError().text();
    }

    return true;
}

QVector<QSharedPointer<AttackMove>> OwnedPokemonAttackMoveDao::populateAttackList(const QSqlQueryModel &attackModel) {
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
    return attackList;
}

QSharedPointer<Pokemon> OwnedPokemonAttackMoveDao::makePokemon(const QSqlRecord &record, const QVector<QSharedPointer<AttackMove>> &attackList) {
    QSharedPointer<Pokemon> pokemon = QSharedPointer<Pokemon>::create(
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
    pokemon->setAttackStatIV(record.value("AttackStatIV").toInt());
    pokemon->setSpAttackStatIV(record.value("SpAttackStatIV").toInt());
    pokemon->setDefenseStatIV(record.value("DefenseStatIV").toInt());
    pokemon->setSpDefenseStatIV(record.value("SpDefenseStatIV").toInt());
    pokemon->setSpeedStatIV(record.value("SpeedStatIV").toInt());
    pokemon->setMaxHealthStatIV(record.value("MaxHealthStatIV").toInt());
    pokemon->setAttackStatEV(record.value("AttackStatEV").toInt());
    pokemon->setSpAttackStatEV(record.value("SpAttackStatEV").toInt());
    pokemon->setDefenseStatEV(record.value("DefenseStatEV").toInt());
    pokemon->setSpDefenseStatEV(record.value("SpDefenseStatEV").toInt());
    pokemon->setSpeedStatEV(record.value("SpeedStatEV").toInt());
    pokemon->setMaxHealthStatEV(record.value("MaxHealthStatEV").toInt());
    pokemon->setStatusCondition(static_cast<Status>(record.value("StatusCondition").toInt()));

    return pokemon;
}
