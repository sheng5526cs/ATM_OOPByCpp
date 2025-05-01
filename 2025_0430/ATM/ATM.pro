QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    loginwindow.cpp

HEADERS += \
    loginwindow.h

FORMS += \
    loginwindow.ui

TRANSLATIONS += \
    ATM_zh_TW.ts
CONFIG += lrelease
CONFIG += embed_translations

RESOURCES += resources.qrc
