#include "controller.h" // 引入登入視窗頭文件
#include <QApplication> // 引入 Qt 應用程式類
#include <QTranslator> // 引入翻譯器類
#include <QLocale> // 引入地區設定類
#include <QFile> // 引入檔案操作類
#include <QTextStream> // 引入文字流類
#include <QDebug> // 引入除錯輸出類
#include <windows.h>

int main(int argc, char *argv[]) // 主函數，程式入口
{
    QApplication a(argc, argv); // 創建 Qt 應用程式物件，處理事件循環

    // 載入翻譯檔案
    QTranslator translator; // 創建翻譯器物件
    const QStringList uiLanguages = QLocale::system().uiLanguages(); // 獲取系統語言清單
    for (const QString &locale : uiLanguages) { // 遍歷語言清單
        const QString baseName = "ATM_" + QLocale(locale).name(); // 構建翻譯檔案名稱
        if (translator.load(":/i18n/" + baseName)) { // 嘗試載入翻譯檔案
            a.installTranslator(&translator); // 安裝翻譯器
            break; // 載入成功後退出循環
        }
    }

    // 載入 QSS 樣式
    QFile file(":/styles/test.qss"); // 開啟樣式檔案
    if (file.open(QFile::ReadOnly | QFile::Text)) { // 以唯讀文字模式開啟
        QTextStream ts(&file); // 創建文字流
        a.setStyleSheet(ts.readAll()); // 讀取並應用樣式
        qDebug() << "QSS loaded and applied!"; // 除錯輸出：樣式載入成功
    } else {
        qDebug() << "Failed to open QSS file."; // 除錯輸出：樣式載入失敗
    }


    Controller controller;   // ✅ 使用 Controller 建立流程
    controller.start();      // ✅ 從 loginWindow 開始顯示
    return a.exec();
}
