#include "loginwindow.h" // 引入 loginWindow 頭文件
#include "ui_loginwindow.h" // 引入 UI 類
#include "controller.h"
#include <QMessageBox> // 引入訊息框類

loginWindow::loginWindow(QWidget *parent) // 建構函數
    : QMainWindow(parent), // 呼叫基類建構函數
    ui(new Ui::loginWindow) // 分配 UI 物件
{
    this->setInputMethodHints(Qt::ImhPreferLatin);
    ui->setupUi(this); // 載入 UI 介面
    ui->PasswordEdit->setEchoMode(QLineEdit::Password); // 設定密碼輸入框為星號顯示
    connect(ui->AccountEdit, &QLineEdit::returnPressed,
            ui->PasswordEdit, QOverload<>::of(&QWidget::setFocus));
    connect(ui->PasswordEdit, &QLineEdit::returnPressed,
            ui->LoginButton, &QPushButton::click);

}

loginWindow::~loginWindow() // 解構函數
{
    delete ui; // 釋放 UI 物件
}


void loginWindow::clearPassword(){
    ui->PasswordEdit->clear(); // 清空密碼欄
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
    emit loginPress(account, password,isAdmin);


}
