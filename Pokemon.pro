QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    battlepage.cpp \
    battleviewmodel.cpp \
    ipage.cpp \
    main.cpp \
    mainmenupage.cpp \
    mainwindow.cpp \
    pagenavigator.cpp \
    pokemon.cpp \
    teammembercard.cpp \
    teampage.cpp \
    trainer.cpp

HEADERS += \
    battlepage.h \
    battleviewmodel.h \
    ipage.h \
    mainmenupage.h \
    mainwindow.h \
    pagenavigator.h \
    pokemon.h \
    teammembercard.h \
    teampage.h \
    trainer.h

FORMS += \
    battlepage.ui \
    mainmenupage.ui \
    mainwindow.ui \
    teammembercard.ui \
    teampage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
