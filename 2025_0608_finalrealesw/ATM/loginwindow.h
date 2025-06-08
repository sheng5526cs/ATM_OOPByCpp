#ifndef LOGINWINDOW_H // 防止頭文件重複包含
#define LOGINWINDOW_H


#include <QMainWindow> // 引入 QMainWindow 基類，提供主視窗功能
#include <QLabel>      // 新增：用於標題和提示標籤
#include <QMessageBox> // 新增：用於美化錯誤提示

QT_BEGIN_NAMESPACE
namespace Ui { class loginWindow; } // 宣告 UI 命名空間，引用 loginwindow.ui 生成的類
QT_END_NAMESPACE

class loginWindow : public QMainWindow { // 定義 loginWindow 類，繼承 QMainWindow
    Q_OBJECT // 啟用 Qt 的信號槽機制

public:
    loginWindow(QWidget *parent = nullptr); // 建構函數，接受父物件指標，預設為空
    ~loginWindow(); // 解構函數，清理資源
    void clearPassword();  // 宣告清除
    
    // === 新增：美化和驗證方法 ===
    void setupUIEnhancements();     // 設定美化界面
    bool validateInput(const QString &account, const QString &password); // 輸入驗證
    void showEnhancedError(const QString &title, const QString &message); // 美化錯誤提示

private slots:
    void on_LoginButton_clicked(); // 私有槽函數，處理 LoginButton 的 clicked 信號
    void on_adminloginButton_clicked();
private:
    Ui::loginWindow *ui; // 指向 UI 物件的指標，用於訪問 loginwindow.ui 的元件

signals:
    void loginPress(const QString &account, const QString &password );  // 新增這個版本
    void adminpress(const QString &account, const QString &password );

};

#endif // LOGINWINDOW_H // 結束頭文件保護
