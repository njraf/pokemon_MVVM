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

    void populateRoutes(QMap<PageName, std::function<QSharedPointer<IPage>(void)> > routes);
    void navigate(PageName page, QVector<QVariant> data = QVector<QVariant>());
    void navigateBack(QVector<QVariant> data = QVector<QVariant>());
    QVector<PageName> getRouteNames();

private:
    explicit PageNavigator(QObject *parent = nullptr);
    QStack<QSharedPointer<IPage>> backstack;
    QMap<PageName, std::function<QSharedPointer<IPage>(void)>> routes;
    QSharedPointer<IPage> prevPage;

signals:
    void pageChanged(QSharedPointer<IPage>);

};

#endif // PAGENAVIGATOR_H
