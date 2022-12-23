#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSharedPointer<Pokemon> charmander = QSharedPointer<Pokemon>::create("Charmander", 1, 1, 1, 1, 1, 1);
    QSharedPointer<Pokemon> squirtle = QSharedPointer<Pokemon>::create("Squirtle", 1, 1, 1, 1, 1, 1);
    QVector<QSharedPointer<Pokemon>> playerTeam = {charmander};
    QVector<QSharedPointer<Pokemon>> opponentTeam = {squirtle};
    QSharedPointer<Trainer> player = QSharedPointer<Trainer>::create(playerTeam);
    QSharedPointer<Trainer> opponent = QSharedPointer<Trainer>::create(opponentTeam);
    QSharedPointer<BattleViewmodel> battleViewmodel = QSharedPointer<BattleViewmodel>::create(player, opponent);

    MainWindow w(battleViewmodel);
    w.show();
    return a.exec();
}
