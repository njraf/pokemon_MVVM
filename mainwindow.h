#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "repository.h"
#include "battlepage.h"
#include "mainmenupage.h"
#include "teampage.h"
#include "summarypage.h"
#include "bagpage.h"
#include "overworldpage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void connectPages(QSharedPointer<IPage> page);
    QSharedPointer<MainMenuPage> constructMainMenuPage() ;
    QSharedPointer<BattlePage> constructBattlePage();
    QSharedPointer<TeamPage> constructTeamPage();
    QSharedPointer<SummaryPage> constructSummaryPage();
    QSharedPointer<BagPage> constructBagPage();
    QSharedPointer<OverworldPage> constructOverworldPage();

private:
    Ui::MainWindow *ui;
    QSharedPointer<Trainer> player;
    QSharedPointer<Repository> repository;

};
#endif // MAINWINDOW_H
