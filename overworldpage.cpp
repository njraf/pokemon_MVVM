#include "overworldpage.h"
#include "ui_overworldpage.h"

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
            QWidget *tileWidget = new QWidget();
            tileWidget->setStyleSheet("border: 1px solid black; background-color: lightgray;");
            gridLayout->addWidget(tileWidget, row, col);

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
    for (int row = -(WORLD_ROWS/2); row < (WORLD_ROWS/2) + 1; row++) {
        QVector<QSharedPointer<Tile>> r;
        for (int col = -(WORLD_COLS/2); col < (WORLD_COLS/2); col++) {
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

            tileWidget = qobject_cast<QWidget*>(item->widget());
            if (tileWidget == nullptr) {
                continue;
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
        break;
    }
}
