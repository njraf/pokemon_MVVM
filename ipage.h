#ifndef IPAGE_H
#define IPAGE_H

#include <QWidget>

enum class PageName : int {
    BATTLE, BAG
};

class IPage : public QWidget
{
    Q_OBJECT
public:
    explicit IPage(QWidget *parent = nullptr);
    virtual ~IPage();
    virtual PageName getPageName() = 0;

    template <typename E>
    static constexpr typename std::underlying_type<E>::type to_underlying(E e) noexcept {
        return static_cast<typename std::underlying_type<E>::type>(e);
    }

signals:
    void changePage(PageName pageNumber);

signals:

};

#endif // IPAGE_H
