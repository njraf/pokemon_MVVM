#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "battlepage.h"
#include "mainmenupage.h"
#include "teampage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QSharedPointer<BattlePage> constructBattlePage();
    QSharedPointer<TeamPage> constructTeamPage();

private:
    Ui::MainWindow *ui;
    QSharedPointer<IPage> currentPage;
    QSharedPointer<Trainer> player;

};
#endif // MAINWINDOW_H
