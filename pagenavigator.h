#ifndef PAGENAVIGATOR_H
#define PAGENAVIGATOR_H

#include <QObject>
#include <QStack>
#include <QSharedPointer>

#include "ipage.h"

class PageNavigator : public QObject
{
    Q_OBJECT
public:
    static PageNavigator *instance;
    static PageNavigator* getInstance();
    ~PageNavigator() = default;
    void addToBackstack(QSharedPointer<IPage> page);
    void popFromBackstack();

private:
    explicit PageNavigator(QObject *parent = nullptr);
    QStack<QSharedPointer<IPage>> backstack;

signals:

};

#endif // PAGENAVIGATOR_H
