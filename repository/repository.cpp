#include "repository.h"

Repository::Repository(QSharedPointer<PokemonDao> pokemonDao_, QSharedPointer<AttackMoveDao> attackMoveDao_, QSharedPointer<HealItemDao> healItemDao_, QObject *parent)
    : QObject(parent)
    , pokemonDao(pokemonDao_)
    , attackMoveDao(attackMoveDao_)
    , healItemDao(healItemDao_)
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

QSharedPointer<AttackMove> Repository::getAttackByID(int id) {
    return attackMoveDao->getAttackByID(id);
}

QSharedPointer<HealItem> Repository::getHealItemByID(int id) {
    return healItemDao->getHealItemByID(id);
}


