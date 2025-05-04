QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    acclistwindow.cpp \
    adminwindow.cpp \
    controller.cpp \
    databasemanager.cpp \
    main.cpp \
    loginwindow.cpp \
    userwindow.cpp

HEADERS += \
    acclistwindow.h \
    adminwindow.h \
    controller.h \
    databasemanager.h \
    loginwindow.h \
    userwindow.h

FORMS += \
    acclistwindow.ui \
    adminwindow.ui \
    loginwindow.ui \
    userwindow.ui

TRANSLATIONS += \
    ATM_zh_TW.ts
CONFIG += lrelease
CONFIG += embed_translations

RESOURCES += resources.qrc

QT += sql
