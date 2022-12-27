#include "pagenavigator.h"

PageNavigator *PageNavigator::instance = nullptr;

PageNavigator::PageNavigator(QObject *parent) : QObject(parent)
{

}

PageNavigator *PageNavigator::getInstance() {
    if (instance == nullptr) {
        instance = new PageNavigator();
    }
    return instance;
}

void PageNavigator::populateRoutes(QMap<PageName, std::function<QSharedPointer<IPage>(QVector<QVariant>)> > routes_) {
    routes = routes_;
}

void PageNavigator::navigate(PageName page, QVector<QVariant> data) {
    if (!routes.contains(page)) {
        return;
    }

    if ((backstack.count() > 1) && (backstack.at(backstack.count() - 2)->getPageName() == page)) {
        navigateBack();
    } else { // make new page
        auto currentPage = routes[page](data);
        backstack.append(currentPage);
        emit pageChanged(currentPage);
    }
}

void PageNavigator::navigateBack(QVector<QVariant> data) {
    if ((backstack.count() > 1)) {
        backstack.pop();
        auto currentPage = backstack.top();
        currentPage->receiveData(data);
        emit pageChanged(currentPage);
    }
}

