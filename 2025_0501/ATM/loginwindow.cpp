#include "loginwindow.h" // 引入 loginWindow 頭文件
#include "ui_loginwindow.h" // 引入 UI 類
#include "user_info.h"
#include <QMessageBox> // 引入訊息框類

loginWindow::loginWindow(QWidget *parent) // 建構函數
    : QMainWindow(parent), // 呼叫基類建構函數
    ui(new Ui::loginWindow) // 分配 UI 物件
{
    ui->setupUi(this); // 載入 UI 介面
    ui->PasswordEdit->setEchoMode(QLineEdit::Password); // 設定密碼輸入框為星號顯示
}

loginWindow::~loginWindow() // 解構函數
{
    delete ui; // 釋放 UI 物件
}

void loginWindow::on_LoginButton_clicked() // 處理登入按鈕點擊
{
    QString account = ui->AccountEdit->text(); // 獲取帳號輸入
    QString password = ui->PasswordEdit->text(); // 獲取密碼輸入
    bool isAdmin = ui->Admin_check->isChecked(); // 檢查管理員勾選框
    qDebug() << "Login button clicked";

    // 檢查輸入是否為空
    if (account.isEmpty() || password.isEmpty()) {
        QMessageBox msgBox(this); // 創建訊息框
        msgBox.setWindowTitle("Error"); // 設定標題
        msgBox.setText("請勿輸入空內容\nPlease fill in all fields!");
        msgBox.setStandardButtons(QMessageBox::Ok); // 設定 OK 按鈕
        msgBox.exec(); // 模態執行
        return; // 終止
    }

    // 硬編碼驗證
    bool loginSuccess = false; // 記錄驗證結果
    if (isAdmin) { // 管理員模式
        loginSuccess = (account == "admin" && password == "admin123"); // 驗證 admin/admin123
    } else { // 用戶模式
        loginSuccess = (account == "user" && password == "1234"); // 驗證 user/1234
    }

    // 處理驗證結果
    if (loginSuccess) {
        Currentuser.setAccount(account);
        Currentuser.setBalance(12345678);
    } else {
        QMessageBox msgBox(this); // 創建訊息框
        msgBox.setWindowTitle("Error"); // 設定標題
        msgBox.setText("Invalid account or password!"); // 設定訊息
        msgBox.setStandardButtons(QMessageBox::Ok); // 設定 OK 按鈕
        msgBox.exec(); // 模態執行
    }
}
