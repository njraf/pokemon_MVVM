#ifndef OVERWORLDPAGE_H
#define OVERWORLDPAGE_H

#include <QWidget>
#include <QKeyEvent>

#include "ipage.h"
#include "overworldviewmodel.h"

namespace Ui {
class OverworldPage;
}

class OverworldPage : public IPage
{
    Q_OBJECT

public:
    explicit OverworldPage(QSharedPointer<OverworldViewmodel> viewmodel_, QWidget *parent = nullptr);
    ~OverworldPage();

    PageName getPageName() override;
    void receiveData(QVector<QVariant> data) override;
    QString tileToColor(QSharedPointer<Tile> tile);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::OverworldPage *ui;
    const int ROWS;
    const int COLS;
    QSharedPointer<OverworldViewmodel> viewmodel;

public slots:
    void drawOverworld(QVector<QVector<QSharedPointer<Tile>>> world);
};

#endif // OVERWORLDPAGE_H
