#include "controller.h"
#include "loginwindow.h"
#include "userwindow.h"
#include "databasemanager.h"
#include "adminwindow.h"
#include "acclistwindow.h"
#include "addaccwindow.h"
#include <QMessageBox>
#include <QCryptographicHash>

// 定義 Controller 類別中的靜態變數
QString Controller::id = "";
QString Controller::account = "";
double Controller::balance = 0.0;

// Controller 建構子，負責初始化各個視窗與訊號連接
Controller::Controller(QObject *parent)
    : QObject(parent)
{
    // 建立登入、使用者與管理員視窗的實體
    loginWin = new loginWindow;
    userWin = new userwindow;
    adminwin = new adminwindow;
    acclist = new acclistWindow;
    addacc = new addaccwindow;


    // 嘗試連接資料庫，失敗則跳出錯誤訊息
    if (!dbm.connect()) {
        QMessageBox::critical(nullptr, "Database Error", "Failed to connect to the database.");
        return;
    }

    // 連接登入視窗的 loginPress 訊號，處理登入邏輯
    connect(loginWin, &loginWindow::loginPress, this, [=](const QString &account, const QString &password,const bool &isadmin) {
        qDebug() << "Controller received  Account:" << account << "Password:" << password;

        bool Login=false;

        // 管理員登入流程
        if(isadmin){
            Login = (account == "admin" && password == "5678");
            if(Login){
                loginWin->hide();      // 隱藏登入視窗
                adminwin->show();      // 顯示管理員視窗
            }
            else{
                // 顯示登入錯誤訊息
                QMessageBox msgBox(loginWin);
                msgBox.setWindowTitle("Error");
                msgBox.setText("Invalid account or password!");
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.exec();
            }
        }
        // 測試帳號 "user" 的登入流程
        else if(account == "user"){
            Login = (account == "user" && password == "1234");
            if(Login){
                qDebug() << "Login press! Account:" << account << "Password:" << password;

                // 設定靜態帳戶資訊
                Controller::Setaccount(account);
                Controller::Setid(password);

                // 確認靜態變數是否正確設定
                qDebug() << "Static Account set to:" << Controller::Getaccount();
                qDebug() << "Static ID set to:" << Controller::Getid();

                loginWin->hide();     // 隱藏登入視窗
                userWin->show();      // 顯示使用者視窗
            }
            else{
                // 顯示登入失敗訊息
                qDebug()<< "Login failed";
                QMessageBox msgBox(loginWin);
                msgBox.setWindowTitle("Error");
                msgBox.setText("Invalid account or password!");
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.exec();
            }
        }
        // 一般用戶（非 admin/user）登入流程，查詢資料庫驗證
        else{
            // 查詢帳密是否正確
            if(dbm.fetchPassword(account,password)){
                // 帳密正確後抓取使用者資料
                if (dbm.fetchUserData(account)) {
                    qDebug() << "User ID:" << Controller::id;
                    qDebug() << "User Balance:" << Controller::balance;
                } else {
                    qDebug() << "Failed to fetch user data.";
                }
                loginWin->hide();     // 隱藏登入視窗
                userWin->show();      // 顯示使用者視窗
            }
            else{
                // 顯示登入失敗訊息
                qDebug()<< "Login failed";
                QMessageBox msgBox(loginWin);
                msgBox.setWindowTitle("Error");
                msgBox.setText("Invalid account or password!");
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.exec();
            }
        }
    });

    // 連接使用者視窗的登出訊號
    connect(userWin, &userwindow::logout, this, [=]() {
        // 清除靜態帳戶與密碼資訊
        Controller::Setaccount("");
        Controller::Setid("");

        // 確認變數是否清空
        qDebug() << "Clear Account set to:" << Controller::Getaccount();
        qDebug() << "Clear ID set to:" << Controller::Getid();

        userWin->hide();            // 隱藏使用者視窗
        loginWin->clearPassword(); // 清除密碼欄位
        loginWin->show();          // 顯示登入視窗
    });

    // 連接管理員視窗的登出訊號
    connect(adminwin, &adminwindow::logout, this, [=](){
        adminwin->hide();          // 隱藏管理員視窗
        loginWin->clearPassword(); // 清除密碼欄位
        loginWin->show();          // 顯示登入視窗
    });

    // 連接管理員視窗切帳戶列表訊號
    connect(adminwin, &adminwindow::Accountlist, this, [=](){
        adminwin->hide();          // 隱藏管理員視窗
        acclist->refresh();
        acclist->show();          // 顯示登入視窗
    });

    connect(adminwin, &adminwindow::addaccountwindow, this, [=](){
        adminwin->hide();          // 隱藏管理員視窗
        addacc->show();          // 顯示登入視窗
    });


    connect(acclist, &acclistWindow::logout, this, [=](){
        acclist->hide();          // 隱藏管理員視窗
        loginWin->clearPassword(); // 清除密碼欄位
        loginWin->show();           // 顯示登入視窗
    });

    connect(acclist, &acclistWindow::back, this, [=](){
        acclist->hide();           // 隱藏管理員視窗
        adminwin->show();          // 顯示登入視窗
    });

    connect(addacc, &addaccwindow::logout, this, [=](){
        addacc->hide();          // 隱藏管理員視窗
        loginWin->clearPassword(); // 清除密碼欄位
        loginWin->show();           // 顯示登入視窗
    });

    connect(addacc, &addaccwindow::back, this, [=](){
        addacc->hide();           // 隱藏管理員視窗
        adminwin->show();          // 顯示登入視窗
    });
}

// 設定靜態帳號變數
void Controller::Setaccount(const QString &newAccount) {
    account = newAccount;
    qDebug() << "Account set to:" << account;
}

// 設定靜態 ID（密碼）變數
void Controller::Setid(const QString &newId) {
    id = newId;
    qDebug() << "ID set to:" << id;
}

// 設定靜態餘額變數
void Controller::Setbalance(double newBalance) {
    balance = newBalance;
    qDebug() << "Balance set to:" << balance;
}

// 取得帳號資訊
QString Controller::Getaccount() {
    return account;
}

// 取得 ID（密碼）資訊
QString Controller::Getid() {
    return id;
}

// 啟動整個系統，顯示登入畫面
void Controller::start() {
    loginWin->show();
}
