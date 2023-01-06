#include "repository.h"

Repository::Repository(QSharedPointer<PokemonDao> pokemonDao_, QSharedPointer<AttackMoveDao> attackMoveDao_, QSharedPointer<HealItemDao> healItemDao_,
                       QSharedPointer<OverworldDao> overworldDao_, QObject *parent)
    : QObject(parent)
    , pokemonDao(pokemonDao_)
    , attackMoveDao(attackMoveDao_)
    , healItemDao(healItemDao_)
    , overworldDao(overworldDao_)
{
    db = QSqlDatabase::addDatabase("QSQLITE", "RepositoryConnection");
    db.setHostName("MyHost");
    db.setDatabaseName("pokemon.db");
    db.setUserName("iamroot");
    db.setPassword("thisisme");
    if (!db.open()) {
        qDebug() << "ERROR: The database could not be opened." << db.lastError().text();
        qDebug() << "Valid database driver" << db.driverName() << db.isDriverAvailable("QSQLITE");
        return;
    }
}

Repository::~Repository() {
    if (db.isOpen()) {
        db.close();
    }
}

bool Repository::hasConnection() {
    return (db.isOpen() && db.isValid());
}

QSharedPointer<Pokemon> Repository::getPokemon(int nationalDexNumber) {
    return pokemonDao->getPokemon(nationalDexNumber);
}

QSharedPointer<AttackMove> Repository::getAttackByID(int id, int effectID) {
    return attackMoveDao->getAttackByID(id, effectID);
}

QSharedPointer<AttackMove> Repository::getAttackByName(QString name, int effectID) {
    return attackMoveDao->getAttackByName(name, effectID);
}

QSharedPointer<HealItem> Repository::getHealItemByID(int id) {
    return healItemDao->getHealItemByID(id);
}

QVector<QVector<QSharedPointer<Tile>>> Repository::getMapByID(int id) {
    return overworldDao->getMapByID(id);
}


