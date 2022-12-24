#include "teammembercard.h"
#include "ui_teammembercard.h"

#include <QDebug>

TeamMemberCard::TeamMemberCard(QSharedPointer<Pokemon> pokemon_, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TeamMemberCard)
    , pokemon(pokemon_)
{
    ui->setupUi(this);

    //ui->healthLabel->setText("");
    //ui->healthbar->setMaximum();
    //ui->healthbar->setValue();
    ui->nameLabel->setText(pokemon->getName());
    //ui->levelLabel->setText();
}

TeamMemberCard::~TeamMemberCard()
{
    delete ui;
}

void TeamMemberCard::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "card clicked";
    emit clicked();
}

