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

void PageNavigator::addToBackstack(QSharedPointer<IPage> page) {
    backstack.push_back(page);
}

void PageNavigator::popFromBackstack() {
    backstack.pop_back();
}




