#ifndef IPAGE_H
#define IPAGE_H

#include <QWidget>
#include <QtSql>
#include <QDebug>

enum class PageName : int {
    BATTLE, BAG, MAIN_MENU, TEAM, POKEMON_SUMMARY
};

class IPage : public QWidget
{
    Q_OBJECT
public:
    explicit IPage(QWidget *parent = nullptr);
    virtual ~IPage() = default;
    virtual PageName getPageName() = 0;

    template <typename E>
    static constexpr typename std::underlying_type<E>::type to_underlying(E e) noexcept {
        return static_cast<typename std::underlying_type<E>::type>(e);
    }

signals:
    void changedPage(PageName pageName); // to a new page. add to backstack.
    void returnedPage(); // to previous page in backstack

};

#endif // IPAGE_H
