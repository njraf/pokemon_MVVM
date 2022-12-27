#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "battlepage.h"
#include "mainmenupage.h"
#include "teampage.h"
#include "repository.h"

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
    QSharedPointer<BattlePage> constructBattlePage();
    QSharedPointer<TeamPage> constructTeamPage();

private:
    Ui::MainWindow *ui;
    QSharedPointer<IPage> currentPage;
    QSharedPointer<Trainer> player;
    QSharedPointer<Repository> repository;

};
#endif // MAINWINDOW_H
