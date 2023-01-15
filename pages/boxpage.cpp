#include "boxpage.h"
#include "ui_boxpage.h"
#include "teammembercard.h"

BoxPage::BoxPage(QSharedPointer<BoxViewmodel> viewmodel_, QWidget *parent) :
    IPage(parent),
    ui(new Ui::BoxPage),
    viewmodel(viewmodel_)
{
    ui->setupUi(this);

    // populate box
    for (int r = 0; r < viewmodel->ROWS; r++) {
        for (int c = 0; c < viewmodel->COLS; c++) {
            auto card = new QLabel("");
            card->setAlignment(Qt::AlignCenter);
            card->setStyleSheet("border: 1px solid black;");
            //connect(card, &QLabel::clicked, this, [=]{});
            ui->boxedPokemonGrid->addWidget(card, r, c);
        }
    }

    drawBox(1);

    QVector<QSharedPointer<Pokemon>> partyPokemon = viewmodel->getPartyPokemon();

    // populate party
    for (auto poke : partyPokemon) {
        ui->partyListArea->addWidget(new TeamMemberCard(poke));
    }

    for (int i = partyPokemon.size(); i < 6; i++) {
        ui->partyListArea->addWidget(new QWidget());
    }

    connect(ui->prevBoxButton, &QPushButton::clicked, this, [=] {
        viewmodel->setCurrentBox(viewmodel->getCurrentBox() - 1);
        int box = viewmodel->getCurrentBox();
        drawBox(box);
        ui->boxNameLabel->setText("Box " + QString::number(box));
    });

    connect(ui->nextBoxButton, &QPushButton::clicked, this, [=] {
        viewmodel->setCurrentBox(viewmodel->getCurrentBox() + 1);
        int box = viewmodel->getCurrentBox();
        drawBox(box);
        ui->boxNameLabel->setText("Box " + QString::number(box));
    });
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

void BoxPage::drawBox(int box) {
    QVector<QSharedPointer<Pokemon>> boxedPokemon = viewmodel->getAllPokemonFromBox(box);
    // populate box
    for (int r = 0; r < viewmodel->ROWS; r++) {
        for (int c = 0; c < viewmodel->COLS; c++) {
            int index = r * viewmodel->COLS + c;

            auto item = ui->boxedPokemonGrid->itemAtPosition(r, c);
            if ((nullptr == item) || (nullptr == item->widget())) {
                continue;
            }

            auto boxWidget = qobject_cast<QLabel*>(item->widget());
            if (nullptr == boxWidget) {
                continue;
            }

            if (index < boxedPokemon.size()) {
                boxWidget->setText(boxedPokemon[index]->getName());
            } else {
                boxWidget->setText("");
            }
        }
    }
}
