#include "controller.h"
#include "loginwindow.h"
#include "userwindow.h"
#include "databasemanager.h"
#include "adminwindow.h"
#include "acclistwindow.h"
#include "addaccwindow.h"
#include "setpasswordwindow.h"
#include "setstatuswindow.h"
#include <QMessageBox>
#include <QCryptographicHash>

// 定義 Controller 類別中的靜態變數
QString Controller::id = "";
QString Controller::account = "";
double Controller::balance = 0.0;

/**
 * @brief Controller 建構子
 * @param parent 父物件指標
 * 
 * 負責初始化各個視窗與建立所有訊號-槽連接
 */
Controller::Controller(QObject *parent)
    : QObject(parent)
{
    // 建立各個視窗實體
    loginWin = new loginWindow;     // 登入視窗
    userWin = new userwindow;       // 使用者視窗
    adminwin = new adminwindow;     // 管理員視窗
    acclist = new acclistWindow;    // 帳戶列表視窗
    addacc = new addaccwindow;      // 新增帳戶視窗
    spw = new setpasswordwindow;    // 密碼設定視窗
    ssw = new setstatuswindow;


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
        loginWin->clearPassword();  // 清除密碼欄位
        loginWin->show();           // 顯示登入視窗
    });

    // 連接管理員視窗的登出訊號
    connect(adminwin, &adminwindow::logout, this, [=](){
        adminwin->hide();           // 隱藏管理員視窗
        loginWin->clearPassword();  // 清除密碼欄位
        loginWin->show();           // 顯示登入視窗
    });

    // 連接管理員視窗與帳戶列表視窗間的訊號
    connect(adminwin, &adminwindow::Accountlist, this, [=](){
        adminwin->hide();           // 隱藏管理員視窗
        acclist->refresh();         // 更新帳戶列表
        acclist->show();            // 顯示帳戶列表視窗
    });

    // 連接管理員視窗與新增帳戶視窗間的訊號
    connect(adminwin, &adminwindow::addaccountwindow, this, [=](){
        adminwin->hide();           // 隱藏管理員視窗
        addacc->show();             // 顯示新增帳戶視窗
    });

    // 連接管理員視窗與密碼設定視窗間的訊號
    connect(adminwin, &adminwindow::passwordsetting, this, [=](){
        adminwin->hide();           // 隱藏管理員視窗
        spw->show();                // 顯示密碼設定視窗
    });

    connect(adminwin, &adminwindow::statuschange, this, [=](){
        adminwin->hide();           // 隱藏管理員視窗
        ssw->show();                // 顯示
    });

    // 連接帳戶列表視窗的登出與返回訊號
    connect(acclist, &acclistWindow::logout, this, [=](){
        acclist->hide();            // 隱藏帳戶列表視窗
        loginWin->clearPassword();  // 清除密碼欄位
        loginWin->show();           // 顯示登入視窗
    });

    connect(acclist, &acclistWindow::back, this, [=](){
        acclist->hide();            // 隱藏帳戶列表視窗
        adminwin->show();           // 顯示管理員視窗
    });

    // 連接新增帳戶視窗的登出與返回訊號
    connect(addacc, &addaccwindow::logout, this, [=](){
        addacc->hide();             // 隱藏新增帳戶視窗
        loginWin->clearPassword();  // 清除密碼欄位
        loginWin->show();           // 顯示登入視窗
    });

    connect(addacc, &addaccwindow::back, this, [=](){
        addacc->hide();             // 隱藏新增帳戶視窗
        adminwin->show();           // 顯示管理員視窗
    });

    // 連接密碼設定視窗的登出與返回訊號
    connect(spw, &setpasswordwindow::logout, this, [=](){
        spw->hide();                // 隱藏密碼設定視窗
        loginWin->clearPassword();  // 清除密碼欄位
        loginWin->show();           // 顯示登入視窗
    });

    connect(spw, &setpasswordwindow::back, this, [=](){
        spw->hide();                // 隱藏密碼設定視窗
        adminwin->show();           // 顯示管理員視窗
    });


    connect(ssw, &setstatuswindow::logout, this, [=](){
        ssw->hide();
        loginWin->clearPassword();  // 清除密碼欄位
        loginWin->show();           // 顯示登入視窗
    });

    connect(ssw, &setstatuswindow::back, this, [=](){
        ssw->hide();                // 隱藏
        adminwin->show();           // 顯示管理員視窗
    });
}

/**
 * @brief 設定靜態帳號變數
 * @param newAccount 新的帳號字串
 */
void Controller::Setaccount(const QString &newAccount) {
    account = newAccount;
    qDebug() << "Account set to:" << account;
}

/**
 * @brief 設定靜態 ID（密碼）變數
 * @param newId 新的 ID 字串
 */
void Controller::Setid(const QString &newId) {
    id = newId;
    qDebug() << "ID set to:" << id;
}

/**
 * @brief 設定靜態餘額變數
 * @param newBalance 新的餘額數值
 */
void Controller::Setbalance(double newBalance) {
    balance = newBalance;
    qDebug() << "Balance set to:" << balance;
}

/**
 * @brief 取得帳號資訊
 * @return 目前設定的帳號字串
 */
QString Controller::Getaccount() {
    return account;
}

/**
 * @brief 取得 ID（密碼）資訊
 * @return 目前設定的 ID 字串
 */
QString Controller::Getid() {
    return id;
}

/**
 * @brief 啟動整個系統
 * 
 * 呼叫此方法來顯示登入畫面，開始程式流程
 */
void Controller::start() {
    loginWin->show();
}
