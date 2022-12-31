#include "overworldpage.h"
#include "ui_overworldpage.h"
#include "pagenavigator.h"

#include <QPushButton>

OverworldPage::OverworldPage(QSharedPointer<OverworldViewmodel> viewmodel_, QWidget *parent) :
    IPage(parent),
    ui(new Ui::OverworldPage),
    ROWS(11),
    COLS(ROWS + (ROWS / 2)),
    viewmodel(viewmodel_)
{
    ui->setupUi(this);

    // populate grid with widgets
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            auto gridLayout = ui->gridLayout;

            if ((row >= ((ROWS / 2) - 1)) && (row <= ((ROWS / 2) + 1)) && (col >= (COLS - 2))) {
                if ((row == ((ROWS / 2) - 1)) && (col == (COLS - 2))) {
                    // create menuStackWidget
                    // grid page
                    menuStackWidget = new QStackedWidget();
                    auto worldGrid = new QGridLayout();

                    worldGrid->addWidget(new QWidget(), 0,0);
                    worldGrid->addWidget(new QWidget(), 1,0);
                    worldGrid->addWidget(new QWidget(), 2,0);
                    worldGrid->addWidget(new QWidget(), 0,1);
                    worldGrid->addWidget(new QWidget(), 1,1);
                    worldGrid->addWidget(new QWidget(), 2,1);
                    QWidget *worldTilePage = new QWidget();
                    worldTilePage->setLayout(worldGrid);

                    // menu page
                    auto menuLayout = new QVBoxLayout();
                    auto pokemonButton = new QPushButton("Pokemon");
                    auto bagButton = new QPushButton("Bag");
                    connect(pokemonButton, &QPushButton::clicked, this, [=] { PageNavigator::getInstance()->navigate(PageName::TEAM); });
                    connect(bagButton, &QPushButton::clicked, this, [=] { PageNavigator::getInstance()->navigate(PageName::BAG); });

                    menuLayout->addWidget(pokemonButton);
                    menuLayout->addWidget(bagButton);
                    QWidget *menuPage = new QWidget();
                    menuPage->setLayout(menuLayout);

                    // add pages to menuStackWidget
                    menuStackWidget->addWidget(worldTilePage);
                    menuStackWidget->addWidget(menuPage);
                    gridLayout->addWidget(menuStackWidget, row, col, 3, 2);
                }
            } else {
                QWidget *tileWidget = new QWidget();
                tileWidget->setStyleSheet("border: 1px solid black; background-color: lightgray;");
                gridLayout->addWidget(tileWidget, row, col);
            }

        }
    }

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
    setFocusPolicy(Qt::StrongFocus);
    menuStackWidget->setCurrentIndex(0);
}

void OverworldPage::drawOverworld(QVector<QVector<QSharedPointer<Tile>>> world) {
    const int WORLD_ROWS = world.size();
    const int WORLD_COLS = world[0].size();
    const int playerRow = viewmodel->getPlayerRow();
    const int playerCol = viewmodel->getPlayerCol();
    auto gridLayout = ui->gridLayout;
    QLayoutItem *item;
    QWidget *tileWidget;

    // window the world into a grid that fits on screen
    QVector<QVector<QSharedPointer<Tile>>> screen;
    for (int row = -(ROWS/2); row < ((ROWS/2) + 1); row++) {
        QVector<QSharedPointer<Tile>> r;
        for (int col = -(COLS/2); col < (COLS/2); col++) {
            int screenRow = playerRow + row;
            int screenCol = playerCol + col;
            if ((screenRow >= 0) && (screenRow < WORLD_ROWS) && (screenCol >= 0) && (screenCol < WORLD_COLS)) {
                r.append(world[screenRow][screenCol]);
            } else {
                r.append(QSharedPointer<Tile>::create(TileType::NONE));
            }
        }
        screen.append(r);
    }

    // draw the window
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            item = gridLayout->itemAtPosition(row, col);
            if ((item == nullptr) || (item->widget() == nullptr)) {
                continue;
            }

            if ((row >= ((ROWS / 2) - 1)) && (row <= ((ROWS / 2) + 1)) && (col >= (COLS - 2))) {
                // draw grid on menuStackWidget
                int stackRow = 2 - ((ROWS / 2) - row + 1);
                int stackCol = 1 - (COLS - col - 1);
                auto page = menuStackWidget->widget(0);
                auto pageGrid = qobject_cast<QGridLayout*>(page->layout());
                item = pageGrid->itemAtPosition(stackRow, stackCol);
                if ((item == nullptr) || (item->widget() == nullptr)) {
                    continue;
                }

                tileWidget = qobject_cast<QWidget*>(item->widget());
                if (tileWidget == nullptr) {
                    continue;
                }
            } else {
                tileWidget = qobject_cast<QWidget*>(item->widget());
                if (tileWidget == nullptr) {
                    continue;
                }
            }

            QSharedPointer<Tile> tile = screen[row][col];
            QString color = tileToColor(tile);

            if ((row == (ROWS/2)) && (col == (COLS/2))) {
                color = "red";
            }

            tileWidget->setStyleSheet("border: 1px solid black; background-color: " + color + ";");
        }
    }
}

QString OverworldPage::tileToColor(QSharedPointer<Tile> tile) {
    QString color = "";
    switch (tile->getType()) {
    case TileType::DIRT:
        color = "brown";
        break;
    case TileType::GRASS:
        color = "limegreen";
        break;
    case TileType::TALL_GRASS:
        color = "green";
        break;
    case TileType::WALL:
        color = "gray";
        break;
    default:
        color = "black";
        break;
    }

    return color;
}

void OverworldPage::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_W:
        viewmodel->move("up");
        break;
    case Qt::Key_A:
        viewmodel->move("left");
        break;
    case Qt::Key_S:
        viewmodel->move("down");
        break;
    case Qt::Key_D:
        viewmodel->move("right");
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter: // keypad
        menuStackWidget->setCurrentIndex(1 - menuStackWidget->currentIndex());
        break;
    }
}
