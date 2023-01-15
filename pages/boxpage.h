#ifndef BOXPAGE_H
#define BOXPAGE_H

#include <QWidget>

#include "ipage.h"
#include "boxviewmodel.h"

namespace Ui {
class BoxPage;
}

class BoxPage : public IPage
{
    Q_OBJECT

public:
    explicit BoxPage(QSharedPointer<BoxViewmodel> viewmodel_, QWidget *parent = nullptr);
    ~BoxPage();

    PageName getPageName() override;
    void receiveData(QVector<QVariant> data) override;
    void drawBox(int box);

private:
    Ui::BoxPage *ui;
    QSharedPointer<BoxViewmodel> viewmodel;
    QSharedPointer<Pokemon> selectedPokemon;

};

#endif // BOXPAGE_H
