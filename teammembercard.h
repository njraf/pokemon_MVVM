#ifndef TEAMMEMBERCARD_H
#define TEAMMEMBERCARD_H

#include <QWidget>

#include "pokemon.h"

namespace Ui {
class TeamMemberCard;
}

class TeamMemberCard : public QWidget
{
    Q_OBJECT

public:
    explicit TeamMemberCard(QSharedPointer<Pokemon> pokemon, QWidget *parent = nullptr);
    ~TeamMemberCard();

    void refresh(QSharedPointer<Pokemon> pokemon);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *) override; // allows styling from qss

private:
    Ui::TeamMemberCard *ui;

signals:
    void clicked();
};

#endif // TEAMMEMBERCARD_H
