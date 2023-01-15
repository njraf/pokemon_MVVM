QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
    repository \
    pages

SOURCES += \
    attackeffectfactory.cpp \
    attackmove.cpp \
    bag.cpp \
    repository/ownedpokemonattackmovedao.cpp \
    repository/ownedpokemondao.cpp \
    pages/boxpage.cpp \
    boxviewmodel.cpp \
    repository/overworlddao.cpp \
    pages/overworldpage.cpp \
    overworldviewmodel.cpp \
    pages/bagpage.cpp \
    bagviewmodel.cpp \
    healitem.cpp \
    repository/attackmovedao.cpp \
    pages/battlepage.cpp \
    battleviewmodel.cpp \
    repository/healitemdao.cpp \
    repository/idao.cpp \
    pages/ipage.cpp \
    main.cpp \
    pages/mainmenupage.cpp \
    mainwindow.cpp \
    natureutilities.cpp \
    pagenavigator.cpp \
    pokemon.cpp \
    pages/summarypage.cpp \
    statuscondition.cpp \
    teammembercard.cpp \
    pages/teampage.cpp \
    tile.cpp \
    trainer.cpp \
    repository/pokemondao.cpp \
    repository/repository.cpp \
    typeutilities.cpp

HEADERS += \
    attackeffectfactory.h \
    attackmove.h \
    bag.h \
    repository/ownedpokemonattackmovedao.h \
    repository/ownedpokemondao.h \
    pages/boxpage.h \
    boxviewmodel.h \
    repository/overworlddao.h \
    pages/overworldpage.h \
    overworldviewmodel.h \
    pages/bagpage.h \
    bagviewmodel.h \
    healitem.h \
    repository/attackmovedao.h \
    pages/battlepage.h \
    battleviewmodel.h \
    repository/healitemdao.h \
    repository/idao.h \
    pages/ipage.h \
    pages/mainmenupage.h \
    mainwindow.h \
    natureutilities.h \
    pagenavigator.h \
    pokemon.h \
    pages/summarypage.h \
    statuscondition.h \
    teammembercard.h \
    pages/teampage.h \
    tile.h \
    trainer.h \
    repository/pokemondao.h \
    repository/repository.h \
    typeutilities.h

FORMS += \
    pages/bagpage.ui \
    pages/battlepage.ui \
    pages/boxpage.ui \
    pages/mainmenupage.ui \
    mainwindow.ui \
    pages/overworldpage.ui \
    pages/summarypage.ui \
    teammembercard.ui \
    pages/teampage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    Resources/resources.qrc
