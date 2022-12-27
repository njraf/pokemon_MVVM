#include "teammembercard.h"
#include "ui_teammembercard.h"

#include <QDebug>

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
}

TeamMemberCard::~TeamMemberCard()
{
    delete ui;
}

void TeamMemberCard::mousePressEvent(QMouseEvent *event)
{
    emit clicked();
}

