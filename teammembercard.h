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
    explicit TeamMemberCard(QSharedPointer<Pokemon> pokemon_, QWidget *parent = nullptr);
    ~TeamMemberCard();

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::TeamMemberCard *ui;
    QSharedPointer<Pokemon> pokemon;

signals:
    void clicked();
};

#endif // TEAMMEMBERCARD_H
