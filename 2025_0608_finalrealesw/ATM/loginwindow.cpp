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

    // === 新增：美化介面設定 ===
    setupUIEnhancements();

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
    ui->AccountEdit->clear();
    ui->AccountEdit->setFocus();
}


void loginWindow::on_LoginButton_clicked() // 處理登入按鈕點擊
{
    QString account = ui->AccountEdit->text(); // 獲取帳號輸入
    QString password = ui->PasswordEdit->text(); // 獲取密碼輸入

    qDebug() << "Login button clicked";

    // 檢查輸入是否為空
    if (account.isEmpty() || password.isEmpty()) {
        showEnhancedError("輸入錯誤", "請勿輸入空內容\nPlease fill in all fields!");
        return; // 終止
    }
    
    // === 新增：輸入驗證 ===
    if (!validateInput(account, password)) {
        return; // 驗證失敗則終止
    }
    
    emit loginPress(account, password);


}


void loginWindow::on_adminloginButton_clicked() // 處理登入按鈕點擊
{
    QString account = ui->AccountEdit->text(); // 獲取帳號輸入
    QString password = ui->PasswordEdit->text(); // 獲取密碼輸入

    qDebug() << "adminloginButton clicked";

    // 檢查輸入是否為空
    if (account.isEmpty() || password.isEmpty()) {
        showEnhancedError("管理員登入錯誤", "請先輸入管理員帳密\nPlease fill in admin credentials!");
        return; // 終止
    }
    
    // === 新增：管理員輸入驗證 ===
    if (!validateInput(account, password)) {
        return; // 驗證失敗則終止
    }
    
    emit adminpress(account, password);
}

// === 新增：美化介面方法 ===
void loginWindow::setupUIEnhancements()
{
    // 設定視窗標題和圖示
    this->setWindowTitle("🏦 ATM Banking System v2.0");
    

    // 添加歡迎文字
    QLabel *welcomeLabel = new QLabel(this);
    welcomeLabel->setText("歡迎使用智能ATM系統 | Welcome to Smart ATM");
    welcomeLabel->setGeometry(400, 500, 480, 30);
    welcomeLabel->setStyleSheet(
        "QLabel {"
        "font: 11pt 'Microsoft YaHei UI';"
        "color: #424242;"
        "background-color: transparent;"
        "}"
    );
    welcomeLabel->setAlignment(Qt::AlignCenter);
    
    // 增強主要背景
    this->setStyleSheet(
        "QMainWindow {"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #E8F5E8, stop:0.5 #F0F0F0, stop:1 #E8F5E8);"
        "}"
    );
    
    // 美化輸入框
    ui->AccountEdit->setPlaceholderText("請輸入帳號 / Enter Account");
    ui->PasswordEdit->setPlaceholderText("請輸入密碼 / Enter Password");
    
    // 增強按鈕樣式
    ui->LoginButton->setStyleSheet(
        "QPushButton {"
        "font: bold 14pt 'Microsoft YaHei UI';"
        "border: 3px solid #1976D2;"
        "border-radius: 12px;"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #E3F2FD, stop:1 #BBDEFB);"
        "color: #0D47A1;"
        "padding: 8px;"
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #BBDEFB, stop:1 #90CAF9);"
        "border-color: #0D47A1;"
        "}"
        "QPushButton:pressed {"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #90CAF9, stop:1 #64B5F6);"
        "}"
    );
    
    // 添加版本信息
    QLabel *versionLabel = new QLabel(this);
    versionLabel->setText("Version 2.0 - Enhanced with Session Management");
    versionLabel->setGeometry(20, 640, 400, 20);
    versionLabel->setStyleSheet(
        "QLabel {"
        "font: 9pt 'Microsoft YaHei UI';"
        "color: #666666;"
        "background-color: transparent;"
        "}"
    );
    
    // 添加安全提示
    QLabel *securityLabel = new QLabel(this);
    securityLabel->setText("🔒 90秒無操作將自動登出 | Auto logout after 90s inactivity");
    securityLabel->setGeometry(750, 640, 500, 20);
    securityLabel->setStyleSheet(
        "QLabel {"
        "font: 9pt 'Microsoft YaHei UI';"
        "color: #E65100;"
        "background-color: transparent;"
        "}"
    );
}

// === 新增：輸入驗證增強 ===
bool loginWindow::validateInput(const QString &account, const QString &password)
{
    // 帳號長度檢查
    if (account.length() < 3) {
        showEnhancedError("帳號長度不足", "Account must be at least 3 characters long!");
        return false;
    }
    
    // 密碼長度檢查
    if (password.length() < 4) {
        showEnhancedError("密碼長度不足", "Password must be at least 4 characters long!");
        return false;
    }
    
    return true;
}

// === 新增：美化錯誤提示 ===
void loginWindow::showEnhancedError(const QString &title, const QString &message)
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("🚫 " + title);
    msgBox.setText(message);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setStyleSheet(
        "QMessageBox {"
        "background-color: #FFF3E0;"
        "}"
        "QMessageBox QLabel {"
        "font: 11pt 'Microsoft YaHei UI';"
        "color: #E65100;"
        "}"
        "QMessageBox QPushButton {"
        "font: 10pt 'Microsoft YaHei UI';"
        "background-color: #FF9800;"
        "color: white;"
        "border: none;"
        "border-radius: 6px;"
        "padding: 6px 12px;"
        "}"
        "QMessageBox QPushButton:hover {"
        "background-color: #F57C00;"
        "}"
    );
    msgBox.exec();
}
