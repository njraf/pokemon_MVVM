#include "boxpage.h"
#include "ui_boxpage.h"
#include "teammembercard.h"
#include "pagenavigator.h"

#include <QMessageBox>

BoxPage::BoxPage(QSharedPointer<BoxViewmodel> viewmodel_, QWidget *parent) :
    IPage(parent),
    ui(new Ui::BoxPage),
    viewmodel(viewmodel_),
    switchMode(false)
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

    QSharedPointer<QVector<QSharedPointer<Pokemon>>> partyPokemon = viewmodel->getPartyPokemon();

    // populate party
    for (auto poke : *partyPokemon) {
        auto card = new TeamMemberCard(poke);
        connect(card, &TeamMemberCard::clicked, this, [=] {
            if (switchMode) {
                switchMode = false;
                viewmodel->swapPartyPosition(selectedPokemon, poke);
                drawBox(viewmodel->getCurrentBox());
                drawParty();
                return;
            }

            QVariant pokeData = QVariant::fromValue<QSharedPointer<Pokemon>>(poke);
            QVector<QVariant> data = {pokeData};
            QPushButton *summaryButton = new QPushButton("Summary");
            QPushButton *switchButton = new QPushButton("Switch");
            QPushButton *itemButton = new QPushButton("Item");
            QPushButton *cancelButton = new QPushButton("Cancel");

            QMessageBox dialog;
            QLayout *layout = dialog.layout();
            QLayoutItem *item;
            while ((item = layout->takeAt(0)) != nullptr) {
                delete item;
            }
            layout->addWidget(summaryButton);
            layout->addWidget(switchButton);
            layout->addWidget(itemButton);
            layout->addWidget(cancelButton);


            dialog.setLayout(layout);

            connect(summaryButton, &QPushButton::clicked, this, [&] { dialog.reject(); PageNavigator::getInstance()->navigate(PageName::POKEMON_SUMMARY, data); });
            connect(switchButton, &QPushButton::clicked, this, [&] {
                dialog.reject();
                card->setStyleSheet("background-color: yellow;");
                switchMode = true;
                selectedPokemon = poke;
            });
            connect(itemButton, &QPushButton::clicked, this, [&] {
                dialog.reject();
                PageNavigator::getInstance()->navigate(PageName::BAG, data);
            });
            connect(cancelButton, &QPushButton::clicked, this, [&] { dialog.reject(); });

            dialog.exec();
        });
        ui->partyListArea->addWidget(card);
    }

    for (int i = partyPokemon->size(); i < 6; i++) {
        ui->partyListArea->addWidget(new QWidget());
    }

    connect(ui->backButton, &QPushButton::clicked, this, [=] { PageNavigator::getInstance()->navigateBack(); });
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
    drawParty();
    drawBox(viewmodel->getCurrentBox());

    if (data.isEmpty()) {
        return;
    }

    if (data[0].canConvert<QSharedPointer<HealItem>>()) { // from BagPage when using A HealItem
        QSharedPointer<HealItem> healItemToUse = data[0].value<QSharedPointer<HealItem>>();
        healItemToUse->use(selectedPokemon);
        drawParty();
    }
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
            boxWidget->setStyleSheet("border: 1px solid black;");
        }
    }
}

void BoxPage::drawParty() {
    QSharedPointer<QVector<QSharedPointer<Pokemon>>> partyPokemon = viewmodel->getPartyPokemon();
    for (int i = 0; i < partyPokemon->size(); i++) {
        auto item = ui->partyListArea->itemAt(i);
        if ((nullptr == item) || (nullptr == item->widget())) {
            continue;
        }

        auto widget = qobject_cast<TeamMemberCard*>(item->widget());
        if (nullptr == widget) {
            continue;
        }
        widget->refresh(partyPokemon->at(i));
        widget->setStyleSheet("");
    }
}
