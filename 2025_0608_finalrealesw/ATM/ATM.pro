QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    acclistwindow.cpp \
    addaccwindow.cpp \
    adminwindow.cpp \
    changepasswordwindow.cpp \
    controller.cpp \
    databasemanager.cpp \
    depositwindow.cpp \
    docuwindow.cpp \
    main.cpp \
    loginwindow.cpp \
    setpasswordwindow.cpp \
    setstatuswindow.cpp \
    summarywindow.cpp \
    transwindow.cpp \
    userwindow.cpp \
    withdrowwindow.cpp

HEADERS += \
    acclistwindow.h \
    addaccwindow.h \
    adminwindow.h \
    changepasswordwindow.h \
    controller.h \
    databasemanager.h \
    depositwindow.h \
    docuwindow.h \
    loginwindow.h \
    setpasswordwindow.h \
    setstatuswindow.h \
    summarywindow.h \
    transwindow.h \
    userwindow.h \
    withdrowwindow.h

FORMS += \
    acclistwindow.ui \
    addaccwindow.ui \
    adminwindow.ui \
    changepasswordwindow.ui \
    depositwindow.ui \
    docuwindow.ui \
    loginwindow.ui \
    setpasswordwindow.ui \
    setstatuswindow.ui \
    summarywindow.ui \
    transwindow.ui \
    userwindow.ui \
    withdrowwindow.ui

TRANSLATIONS += \
    ATM_zh_TW.ts
CONFIG += lrelease
CONFIG += embed_translations

RESOURCES += resources.qrc

QT += sql
