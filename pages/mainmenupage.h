#ifndef MAINMENUPAGE_H
#define MAINMENUPAGE_H

#include <QWidget>

#include "ipage.h"

namespace Ui {
class MainMenuPage;
}

class MainMenuPage : public IPage
{
    Q_OBJECT

public:
    explicit MainMenuPage(QWidget *parent = nullptr);
    ~MainMenuPage();
    PageName getPageName() override;
    void receiveData(QVector<QVariant> data) override;

private:
    Ui::MainMenuPage *ui;

signals:
    void quit();
};

#endif // MAINMENUPAGE_H
