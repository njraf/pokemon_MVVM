#include "teammembercard.h"
#include "ui_teammembercard.h"
#include "pagenavigator.h"
#include "statusutilities.h"

#include <QVariant>
#include <QDebug>
#include <QPainter>

TeamMemberCard::TeamMemberCard(QSharedPointer<Pokemon> pokemon_, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TeamMemberCard)
    , pokemon(pokemon_)
{
    ui->setupUi(this);

    int health = pokemon->getHealthStat();
    int maxHealth = pokemon->getMaxHealthStat();
    ui->healthLabel->setText(QString("HP: %1/%2").arg(health).arg(maxHealth));
    ui->healthbar->setMaximum(maxHealth);
    ui->healthbar->setValue(health);
    ui->nameLabel->setText(pokemon->getName());
    ui->levelLabel->setText(QString("L: %1").arg(pokemon->getLevel()));
    ui->statusConditionLabel->setText(statusToName(pokemon->getStatusCondition()));
    ui->statusConditionLabel->setStyleSheet("background-color: " + statusToColor(pokemon->getStatusCondition()) + ";");
}

TeamMemberCard::~TeamMemberCard()
{
    delete ui;
}

void TeamMemberCard::mousePressEvent(QMouseEvent *event)
{
    emit clicked();
}

void TeamMemberCard::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
