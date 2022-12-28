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
    attackmove.cpp \
    repository/attackmovedao.cpp \
    pages/battlepage.cpp \
    battleviewmodel.cpp \
    repository/idao.cpp \
    pages/ipage.cpp \
    main.cpp \
    pages/mainmenupage.cpp \
    mainwindow.cpp \
    natureutilities.cpp \
    pagenavigator.cpp \
    pokemon.cpp \
    pages/summarypage.cpp \
    teammembercard.cpp \
    pages/teampage.cpp \
    trainer.cpp \
    repository/pokemondao.cpp \
    repository/repository.cpp \
    typeutilities.cpp

HEADERS += \
    attackmove.h \
    repository/attackmovedao.h \
    pages/battlepage.h \
    battleviewmodel.h \
    repository/idao.h \
    pages/ipage.h \
    pages/mainmenupage.h \
    mainwindow.h \
    natureutilities.h \
    pagenavigator.h \
    pokemon.h \
    pages/summarypage.h \
    teammembercard.h \
    pages/teampage.h \
    trainer.h \
    repository/pokemondao.h \
    repository/repository.h \
    typeutilities.h

FORMS += \
    battlepage.ui \
    mainmenupage.ui \
    mainwindow.ui \
    summarypage.ui \
    teammembercard.ui \
    teampage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    Resources/resources.qrc
