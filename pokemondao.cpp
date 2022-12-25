#include "pokemondao.h"

PokemonDao::PokemonDao(QObject *parent) : IDao(parent)
{

}

QSharedPointer<Pokemon> PokemonDao::getPokemon(int nationalDexNumber) {
    if (!db.isOpen()) {
        qDebug() << "pokeDao database is not opened";
    }
    QSqlTableModel model(nullptr, db);
    model.setTable("Pokemon");
    model.setFilter(QString("NatDexNumber == %1").arg(nationalDexNumber));
    model.select();

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


