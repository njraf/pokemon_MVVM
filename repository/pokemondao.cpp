#include "pokemondao.h"
#include "attackeffectfactory.h"

PokemonDao::PokemonDao(QObject *parent) : IDao(parent)
{
    db = QSqlDatabase::addDatabase(SQLITE_CONN, "PokemonDaoConnection");
    db.setHostName("MyHost");
    db.setDatabaseName("pokemon.db");
    db.setUserName("iamroot");
    db.setPassword("thisisme");
    if (!db.open()) {
        qDebug() << "ERROR: The PokemonDao database could not be opened." << db.lastError().text();
        qDebug() << "Valid database driver" << db.driverName() << db.isDriverAvailable(SQLITE_CONN);
        return;
    }

    populateDatabase();
}

QSharedPointer<Pokemon> PokemonDao::getPokemon(int nationalDexNumber) {
    if (!db.isOpen()) {
        qDebug() << "PokemonDao database is not opened";
    }
    QSqlTableModel model(nullptr, db);
    model.setTable("Pokemon");
    model.setFilter(QString("NatDexNumber=%1").arg(nationalDexNumber));
    if (!model.select()) {
        qDebug() << "ERROR: getPokemon()::select() failed" << db.lastError().text();
    }

    auto record = model.record(0);
    QVector<QSharedPointer<AttackMove>> attackList = {
        //QSharedPointer<AttackMove>::create("Ember", 40, 100, 25, 25, Type::FIRE, Category::SPECIAL, AttackEffectFactory::getEffectByID(-1))

    };

    // make a new ID for the pokemon
    int newID = 1;
    QSqlTableModel ownedPokemonModel(nullptr, db);
    ownedPokemonModel.setTable("OwnedPokemon");
    ownedPokemonModel.select();

    if (ownedPokemonModel.rowCount() > 0) {
        QSqlQuery query(db);
        if (!query.exec("SELECT MAX(ID) FROM OwnedPokemon")) {
            qDebug() << "PokemonDao query failed " << query.lastError().text();
        }
        query.next();
        newID = query.value(0).toInt() + 1;
    }

    return QSharedPointer<Pokemon>::create(
                newID,
                nationalDexNumber,
                record.value("Name").toString(),
                "",
                -1,
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
    //qDebug() << "Populating Pokemon database";
    QSqlQuery query(db);
    if (!query.exec("DROP TABLE Pokemon;")) {
        qDebug() << "Drop table failed" << db.lastError().text();
    }
    if (!query.exec("CREATE TABLE Pokemon (NatDexNumber int PRIMARY_KEY, Name varchar(32), BaseHealth int, BaseAttack int, BaseDefense int, BaseSpAttack int, BaseSpDefense int, BaseSpeed int, Type1 int, Type2 int);")) {
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

    // read from a csv file to populate the database
    QFile pokemonFile("repository/pokemon.csv");
    if (!pokemonFile.exists() || !pokemonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "ERROR:" << pokemonFile.fileName() << "does not exist";
    } else {
        int row = table.rowCount();
        while (!pokemonFile.atEnd()) {
            table.insertRow(row);
            QString line = pokemonFile.readLine();
            QStringList parts = line.split(',');
            if (line.isEmpty() || (parts.count() != table.columnCount())) {
                continue;
            }

            for (int col = 0; col < parts.size(); col++) {
                QVariant data(parts[col].simplified());
                if ((col == 0) || ((col > 1) && (col < 8))) { // int data
                    if (!table.setData(table.index(row, col), data.toInt())) {
                        qDebug() << "ERROR: Could not set int data" << db.lastError().text();
                    }
                } else { // string data
                    QString strData = data.toString();
                    if (TypeUtilities::strToUnderlying.contains(strData)) {
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
    pokemonFile.close();

    if (!table.submitAll()) {
        qDebug() << "ERROR: Last submitAll() error." << db.lastError().text();
        return false;
    }

    return true;
}

