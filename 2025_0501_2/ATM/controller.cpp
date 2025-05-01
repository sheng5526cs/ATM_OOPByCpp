#include "controller.h"
#include "loginwindow.h"  // 需要具體實現的頭文件
#include "userwindow.h"   // 需要具體實現的頭文件

QString Controller::id = "";
QString Controller::account = "";
double Controller::balance = 0.0;



Controller::Controller(QObject *parent)
    : QObject(parent)
{
    loginWin = new loginWindow;  // 在這裡使用具體類別
    userWin = new userwindow;    // 在這裡使用具體類別

    // 連接登入成功訊號，傳送帳戶資訊並更新靜態變數
    connect(loginWin, &loginWindow::loginSuccess, this, [=](const QString &account, const QString &password) {
        qDebug() << "Controller received login success! Account:" << account << "Password:" << password;

        // 設置靜態帳戶資訊
        Controller::Setaccount(account);
        Controller::Setid(password);      // 設置靜態密碼資訊（如果需要的話）

        // 檢查靜態變數是否設置正確
        qDebug() << "Static Account set to:" << Controller::Getaccount();
        qDebug() << "Static ID set to:" << Controller::Getid();

        loginWin->hide();
        userWin->show();
    });

    // 連接登出訊號，切換回登入畫面
    connect(userWin, &userwindow::logout, this, [=]() {

        // 設置靜態帳戶資訊
        Controller::Setaccount("");
        Controller::Setid("");      // 設置靜態密碼資訊（如果需要的話）

        // 檢查靜態變數是否設置正確
        qDebug() << "Clear Account set to:" << Controller::Getaccount();
        qDebug() << "Clear ID set to:" << Controller::Getid();


        userWin->hide();
        loginWin->clearPassword();
        loginWin->show();
    });
}

void Controller::Setaccount(const QString &newAccount) {
    account = newAccount;
    qDebug() << "Account set to:" << account;
}

void Controller::Setid(const QString &newId) {
    id = newId;
    qDebug() << "ID set to:" << id;
}

QString Controller::Getaccount() {
    return account;
}

QString Controller::Getid() {
    return id;
}

void Controller::start() {
    loginWin->show(); // 顯示登入畫面
}
