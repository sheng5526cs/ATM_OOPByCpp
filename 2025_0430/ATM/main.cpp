#include "loginwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 翻譯器
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "ATM_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    QFile file(":/styles/test.qss");
    if (file.exists()) {
        qDebug() << "QSS file exists!";
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream ts(&file);
            QString styleSheet = ts.readAll();
            qApp->setStyleSheet(styleSheet);
            qDebug() << "QSS loaded and applied!";
        } else {
            qDebug() << "Failed to open QSS file.";
        }
    } else {
        qDebug() << "QSS file not found in resources!";
    }
    qDebug() << "Resource paths in /styles:";
    QDir dir(":/styles");
    QStringList entries = dir.entryList();
    for (const QString &entry : entries) {
        qDebug() << entry;
    }


    loginWindow w;
    w.show();
    return a.exec();
}
