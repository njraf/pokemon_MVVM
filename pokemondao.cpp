#include "pokemondao.h"

PokemonDao::PokemonDao(QObject *parent) : IDao(parent)
{
    db = QSqlDatabase::addDatabase(SQLITE_CONN, "PokemonDaoConnection");
    db.setHostName("MyHost");
    db.setDatabaseName("pokemon.db");
    db.setUserName("iamroot");
    db.setPassword("thisisme");
    if (!db.open()) {
        qDebug() << "ERROR: The IDao database could not be opened." << db.lastError().text();
        qDebug() << "Valid database driver" << db.driverName() << db.isDriverAvailable(SQLITE_CONN);
        return;
    }

    populateDatabase();
}

QSharedPointer<Pokemon> PokemonDao::getPokemon(int nationalDexNumber) {
    if (!db.isOpen()) {
        qDebug() << "pokeDao database is not opened";
    }
    QSqlTableModel model(nullptr, db);
    model.setTable("Pokemon");
    model.setFilter(QString("NatDexNumber=%1").arg(nationalDexNumber));
    if (!model.select()) {
        qDebug() << "ERROR: getPokemon()::select() failed" << db.lastError().text();
    }

    auto record = model.record(0);
    QVector<QSharedPointer<AttackMove>> attackList = {
        QSharedPointer<AttackMove>::create("Ember", 40, 100, 25, 25, Type::FIRE, Category::SPECIAL)
    };

    return QSharedPointer<Pokemon>::create(
                record.value("Name").toString(),
                "",
                NatureUtilities::randomNature(),
                record.value("BaseHealth").toInt(),
                record.value("BaseAttack").toInt(),
                record.value("BaseDefense").toInt(),
                record.value("BaseSpAttack").toInt(),
                record.value("BaseSpDefense").toInt(),
                record.value("BaseSpeed").toInt(),
                1000,
                5,
                attackList,
                static_cast<Type>(record.value("Type1").toInt()),
                static_cast<Type>(record.value("Type2").toInt())
                );
}


bool PokemonDao::populateDatabase() {
    const int NUM_POKEMON_TO_INSERT = 6;
    QSqlQuery query(db);
    if (!query.exec("DROP TABLE Pokemon;")) {
        qDebug() << "Drop table failed" << db.lastError().text();
    }
    if (!query.exec("CREATE TABLE Pokemon (NatDexNumber int, Name varchar(32), BaseHealth int, BaseAttack int, BaseDefense int, BaseSpAttack int, BaseSpDefense int, BaseSpeed int, Type1 int, Type2 int);")) {
        qDebug() << "Create table failed" << db.lastError().text();
    }

    int col = 0;
    QSqlTableModel table(nullptr, db);
    table.setTable("Pokemon");
    table.setEditStrategy(QSqlTableModel::OnManualSubmit);
    if (!table.select()) {
        qDebug() << "ERROR: First select() error." << db.lastError().text();
        return false;
    }

    table.setHeaderData(col++, Qt::Horizontal, "NatDexNumber");
    table.setHeaderData(col++, Qt::Horizontal, "Name");
    table.setHeaderData(col++, Qt::Horizontal, "BaseHealth");
    table.setHeaderData(col++, Qt::Horizontal, "BaseAttack");
    table.setHeaderData(col++, Qt::Horizontal, "BaseDefense");
    table.setHeaderData(col++, Qt::Horizontal, "BaseSpAttack");
    table.setHeaderData(col++, Qt::Horizontal, "BaseSpDefense");
    table.setHeaderData(col++, Qt::Horizontal, "BaseSpeed");
    table.setHeaderData(col++, Qt::Horizontal, "Type1");
    table.setHeaderData(col++, Qt::Horizontal, "Type2");
    if (!table.select()) {
        qDebug() << "ERROR: Second select() error." << db.lastError().text();
        return false;
    }

    if (table.rowCount() < NUM_POKEMON_TO_INSERT) {
        table.insertRows(0, NUM_POKEMON_TO_INSERT);
    }


    table.setData(table.index(0, 0), 1);
    table.setData(table.index(0, 1), "Bulbasaur");
    table.setData(table.index(0, 2), 45);
    table.setData(table.index(0, 3), 49);
    table.setData(table.index(0, 4), 49);
    table.setData(table.index(0, 5), 65);
    table.setData(table.index(0, 6), 65);
    table.setData(table.index(0, 7), 45);
    table.setData(table.index(0, 8), TypeUtilities::to_underlying(Type::GRASS));
    table.setData(table.index(0, 9), TypeUtilities::to_underlying(Type::POISON));

    table.setData(table.index(1, 0), 2);
    table.setData(table.index(1, 1), "Ivysaur");
    table.setData(table.index(1, 2), 60);
    table.setData(table.index(1, 3), 62);
    table.setData(table.index(1, 4), 63);
    table.setData(table.index(1, 5), 80);
    table.setData(table.index(1, 6), 80);
    table.setData(table.index(1, 7), 60);
    table.setData(table.index(1, 8), TypeUtilities::to_underlying(Type::GRASS));
    table.setData(table.index(1, 9), TypeUtilities::to_underlying(Type::POISON));

    table.setData(table.index(2, 0), 3);
    table.setData(table.index(2, 1), "Venusaur");
    table.setData(table.index(2, 2), 80);
    table.setData(table.index(2, 3), 82);
    table.setData(table.index(2, 4), 83);
    table.setData(table.index(2, 5), 100);
    table.setData(table.index(2, 6), 100);
    table.setData(table.index(2, 7), 80);
    table.setData(table.index(2, 8), TypeUtilities::to_underlying(Type::GRASS));
    table.setData(table.index(2, 9), TypeUtilities::to_underlying(Type::POISON));

    table.setData(table.index(3, 0), 4);
    table.setData(table.index(3, 1), "Charmander");
    table.setData(table.index(3, 2), 39);
    table.setData(table.index(3, 3), 52);
    table.setData(table.index(3, 4), 43);
    table.setData(table.index(3, 5), 60);
    table.setData(table.index(3, 6), 50);
    table.setData(table.index(3, 7), 65);
    table.setData(table.index(3, 8), TypeUtilities::to_underlying(Type::FIRE));
    table.setData(table.index(3, 9), TypeUtilities::to_underlying(Type::NONE));

    table.setData(table.index(4, 0), 5);
    table.setData(table.index(4, 1), "Charmeleon");
    table.setData(table.index(4, 2), 58);
    table.setData(table.index(4, 3), 64);
    table.setData(table.index(4, 4), 58);
    table.setData(table.index(4, 5), 80);
    table.setData(table.index(4, 6), 65);
    table.setData(table.index(4, 7), 80);
    table.setData(table.index(4, 8), TypeUtilities::to_underlying(Type::FIRE));
    table.setData(table.index(4, 9), TypeUtilities::to_underlying(Type::NONE));

    table.setData(table.index(5, 0), 6);
    table.setData(table.index(5, 1), "Charizard");
    table.setData(table.index(5, 2), 78);
    table.setData(table.index(5, 3), 84);
    table.setData(table.index(5, 4), 78);
    table.setData(table.index(5, 5), 109);
    table.setData(table.index(5, 6), 85);
    table.setData(table.index(5, 7), 100);
    table.setData(table.index(5, 8), TypeUtilities::to_underlying(Type::FIRE));
    table.setData(table.index(5, 9), TypeUtilities::to_underlying(Type::FLYING));


    if (!table.submitAll()) {
        qDebug() << "ERROR: Last submitAll() error." << db.lastError().text();
        return false;
    }

    return true;
}

