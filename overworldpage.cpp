#include "overworldpage.h"
#include "ui_overworldpage.h"

OverworldPage::OverworldPage(QSharedPointer<OverworldViewmodel> viewmodel_, QWidget *parent) :
    IPage(parent),
    ui(new Ui::OverworldPage),
    viewmodel(viewmodel_)
{
    ui->setupUi(this);

    connect(viewmodel.data(), &OverworldViewmodel::worldUpdated, this, &OverworldPage::drawOverworld);
    drawOverworld(viewmodel->getWorld());
}

OverworldPage::~OverworldPage()
{
    delete ui;
}

PageName OverworldPage::getPageName() {
    return PageName::OVERWORLD;
}

void OverworldPage::receiveData(QVector<QVariant> data) {

}

void OverworldPage::drawOverworld(QVector<QVector<QSharedPointer<Tile>>> world) {
    const int playerRow = viewmodel->getPlayerRow();
    const int playerCol = viewmodel->getPlayerCol();
    auto gridLayout = ui->gridLayout;
    QLayoutItem *item;
    while ((item = gridLayout->takeAt(0)) != nullptr) {
        delete item;
    }

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            QSharedPointer<Tile> tile = world[row][col];
            QString color = "";
            switch (tile->getType()) {
            case TileType::DIRT:
                color = "brown";
                break;
            case TileType::GRASS:
                color = "limegreen";
                break;
            case TileType::WALL:
                color = "gray";
                break;
            default:
                color = "black";
                break;
            }

            if ((row == playerRow) && (col == playerCol)) {
                color = "red";
            }

            QWidget *tileWidget = new QWidget();
            tileWidget->setStyleSheet("border: 1px solid black; background-color: " + color + ";");
            gridLayout->addWidget(tileWidget, row, col);

        }
    }
}
