#include "boxviewmodel.h"

BoxViewmodel::BoxViewmodel(QSharedPointer<Repository> repository_,QObject *parent)
    : QObject(parent)
    , repository(repository_)
{

}
