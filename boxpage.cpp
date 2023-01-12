#include "boxpage.h"
#include "ui_boxpage.h"
#include "teammembercard.h"

BoxPage::BoxPage(QSharedPointer<BoxViewmodel> viewmodel_, QWidget *parent) :
    IPage(parent),
    ui(new Ui::BoxPage),
    viewmodel(viewmodel_)
{
    ui->setupUi(this);

    //TODO: get pokemon from the repository
    QVector<QSharedPointer<Pokemon>> boxedPokemon; // 2d vector?

    // populate box
    for (int r = 0; r < viewmodel->ROWS; r++) {
        for (int c = 0; c < viewmodel->COLS; c++) {
            ui->boxedPokemonGrid->addWidget(new QWidget(), r, c);
        }
    }

    //TODO: get party pokemon from the repository
    QVector<QSharedPointer<Pokemon>> partyPokemon;

    // populate party
    for (auto poke : partyPokemon) {
        ui->partyListArea->addWidget(new TeamMemberCard(poke));
    }

    for (int i = partyPokemon.size(); i < 6; i++) {
        ui->partyListArea->addWidget(new QWidget());
    }

}

BoxPage::~BoxPage()
{
    delete ui;
}

PageName BoxPage::getPageName() {
    return PageName::BOX;
}

void BoxPage::receiveData(QVector<QVariant> data) {

}
