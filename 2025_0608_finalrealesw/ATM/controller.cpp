#include "controller.h"
#include "loginwindow.h"
#include "userwindow.h"
#include "databasemanager.h"
#include "adminwindow.h"
#include "acclistwindow.h"
#include "addaccwindow.h"
#include "setpasswordwindow.h"
#include "setstatuswindow.h"
#include "changepasswordwindow.h"
#include "withdrowwindow.h"
#include "docuwindow.h"
#include "depositwindow.h"
#include "transwindow.h"
#include "summarywindow.h"
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
    changepw = new changepasswordWindow;
    withdrowwin = new withdrowWindow; // 新增：提款視窗
    docuwin = new docuWindow; // 新增：交易紀錄視窗
    depositwin = new depositWindow; // 新增：存款視窗
    transwin = new TransWindow;
    summaryWin = new summarywindow; // 新增：摘要視窗


    // 嘗試連接資料庫，失敗則跳出錯誤訊息
    if (!dbm.connect()) {
        QMessageBox::critical(nullptr, "Database Error", "Failed to connect to the database.");
        return;
    }

    // 連接登入視窗的 loginPress 訊號，處理登入邏輯
    connect(loginWin, &loginWindow::loginPress, this, [=](const QString &account, const QString &password) {
        qDebug() << "Controller received  Account:" << account << "Password:" << password;


        // 一般用戶（非 admin/user）登入流程，查詢資料庫驗證

            int result = dbm.fetchPassword(account, password);

            if (result == 1) {
                // Login successful
                qDebug() << "[DEBUG] Login successful";

                if (dbm.fetchUserData(account)) {
                    qDebug() << "[DEBUG] User ID:" << Controller::id;
                    qDebug() << "[DEBUG] User Balance:" << Controller::balance;
                } else {
                    qDebug() << "[DEBUG] Failed to fetch user data";
                }

                loginWin->hide();     // 隱藏登入視窗
                userWin->setWelcomeText(account); // 新增：顯示歡迎訊息
                userWin->show();      // 顯示使用者視窗
                sessionActive = true;
                sessionTimer->start();

            } else if (result == 9) {
                QMessageBox::warning(loginWin, "帳號鎖定",
                                     "由於多次登入失敗，您的帳號已被鎖定，請聯絡管理員。");
                loginWin->clearPassword();

            } else if (result == 0) {
                QMessageBox msgBox(loginWin);
                msgBox.setWindowTitle("登入失敗");
                msgBox.setText("密碼錯誤，請再試一次。");
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.exec();
                loginWin->clearPassword();

            } else if (result == -2) {
                QMessageBox::warning(loginWin, "帳號不存在",
                                     "查無此帳號，請確認輸入是否正確。");
                loginWin->clearPassword();

            } else if (result == -1) {
                QMessageBox::critical(loginWin, "資料庫錯誤",
                                      "無法連線至資料庫，請稍後再試。");
                loginWin->clearPassword();
            } else if (result == 500||result == 501) {
                QMessageBox::critical(loginWin,
                                      QStringLiteral("連接錯誤"),
                                      QStringLiteral("管理員請使用管理模式登入。\n非管理員請勿嘗試登入。\n本系統與警局時刻連線中。"));
                loginWin->clearPassword();
            }




    });


    connect(loginWin, &loginWindow::adminpress, this, [=](const QString &account, const QString &password) {
        qDebug() << "Controller received  Account:" << account << "Password:" << password;
            int result = dbm.fetchPassword(account, password);

            if (result == 501) {
                // Login successful
                qDebug() << "[DEBUG] Login successful";

                if (dbm.fetchUserData(account)) {
                    qDebug() << "[DEBUG] User ID:" << Controller::id;
                    qDebug() << "[DEBUG] User Balance:" << Controller::balance;
                } else {
                    qDebug() << "[DEBUG] Failed to fetch user data";
                }

                loginWin->hide();     // 隱藏登入視窗
                adminwin->show();      // 顯示視窗
                sessionActive = true;
                sessionTimer->start();

            } else {
                QMessageBox::critical(loginWin,
                                      QStringLiteral("連接錯誤"),
                                      QStringLiteral("密碼錯誤。\n非管理員請勿嘗試登入。\n本系統與警局時刻連線中。"));
                loginWin->clearPassword();
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
        sessionActive = false;      // 添加這行
        sessionTimer->stop();       // 添加這行
    });

    connect(userWin, &userwindow::changepass,this,[=](){
        userWin->hide();
        changepw->show();
        resetSessionTimer(); // 添加這行
    });

    // 連接使用者視窗的存款訊號
    connect(userWin, &userwindow::deposit, this, [=](){
        userWin->hide();
        depositwin->refreshBalance();
        depositwin->show();
        resetSessionTimer(); // 添加這行
    });
    // 連接存款視窗的返回與登出訊號
    connect(depositwin, &depositWindow::back, this, [=](){
        depositwin->hide();
        userWin->setWelcomeText(Controller::Getaccount());
        userWin->show();
        resetSessionTimer(); // 添加這行
    });
    connect(depositwin, &depositWindow::logout, this, [=](){
        depositwin->hide();
        Controller::Setaccount("");
        Controller::Setid("");
        loginWin->clearPassword();
        loginWin->show();
        sessionActive = false;      // 添加這行
        sessionTimer->stop();       // 添加這行
    });

    // 連接使用者視窗的提款訊號
    connect(userWin, &userwindow::withdraw, this, [=](){
        userWin->hide();
        withdrowwin->refreshBalance(); // 進入前刷新餘額
        withdrowwin->show();
        resetSessionTimer(); // 添加這行
    });
    // 連接提款視窗的返回與登出訊號
    connect(withdrowwin, &withdrowWindow::back, this, [=](){
        withdrowwin->hide();
        userWin->setWelcomeText(Controller::Getaccount());
        userWin->show();
        resetSessionTimer(); // 添加這行
    });
    connect(withdrowwin, &withdrowWindow::logout, this, [=](){
        withdrowwin->hide();
        Controller::Setaccount("");
        Controller::Setid("");
        loginWin->clearPassword();
        loginWin->show();
        sessionActive = false;      // 添加這行
        sessionTimer->stop();       // 添加這行
    });

    // 連接使用者視窗的轉帳訊號
    connect(userWin, &userwindow::transfer, this, [=](){
        userWin->hide();
        transwin->refreshBalance();
        transwin->show();
        resetSessionTimer(); // 添加這行
    });
    // 連接轉帳視窗的返回與登出訊號
    if (transwin == nullptr) transwin = new TransWindow;
    connect(transwin, &TransWindow::back, this, [=](){
        transwin->hide();
        userWin->setWelcomeText(Controller::Getaccount());
        userWin->show();
        resetSessionTimer(); // 添加這行
    });
    connect(transwin, &TransWindow::logout, this, [=](){
        transwin->hide();
        Controller::Setaccount("");
        Controller::Setid("");
        loginWin->clearPassword();
        loginWin->show();
        sessionActive = false;      // 添加這行
        sessionTimer->stop();       // 添加這行
    });

    // 連接使用者視窗的交易紀錄訊號
    connect(userWin, &userwindow::transactionHistory, this, [=](){
        userWin->hide();
        docuwin->setAccountAndBalance(Controller::Getaccount(), Controller::Getbalance());
        docuwin->refreshTable(Controller::Getaccount());
        docuwin->show();
        resetSessionTimer(); // 添加這行
    });
    // 連接 docuwindow 的返回與登出訊號
    connect(docuwin, &docuWindow::back, this, [=](){
        docuwin->hide();
        userWin->setWelcomeText(Controller::Getaccount());
        userWin->show();
        resetSessionTimer(); // 添加這行
    });
    connect(docuwin, &docuWindow::logout, this, [=](){
        docuwin->hide();
        Controller::Setaccount("");
        Controller::Setid("");
        loginWin->clearPassword();
        loginWin->show();
        sessionActive = false;      // 添加這行
        sessionTimer->stop();       // 添加這行
    });

    // 連接管理員視窗的登出訊號
    connect(adminwin, &adminwindow::logout, this, [=](){
        adminwin->hide();           // 隱藏管理員視窗
        loginWin->clearPassword();  // 清除密碼欄位
        loginWin->show();           // 顯示登入視窗
        sessionActive = false;      // 添加這行
        sessionTimer->stop();       // 添加這行
    });

    // 連接管理員視窗與帳戶列表視窗間的訊號
    connect(adminwin, &adminwindow::Accountlist, this, [=](){
        adminwin->hide();           // 隱藏管理員視窗
        acclist->refresh();         // 更新帳戶列表
        acclist->show();            // 顯示帳戶列表視窗
        resetSessionTimer(); // 添加這行
    });

    // 連接管理員視窗與新增帳戶視窗間的訊號
    connect(adminwin, &adminwindow::addaccountwindow, this, [=](){
        adminwin->hide();           // 隱藏管理員視窗
        addacc->show();             // 顯示新增帳戶視窗
        resetSessionTimer(); // 添加這行
    });

    // 連接管理員視窗與密碼設定視窗間的訊號
    connect(adminwin, &adminwindow::passwordsetting, this, [=](){
        adminwin->hide();           // 隱藏管理員視窗
        spw->show();                // 顯示密碼設定視窗
        resetSessionTimer(); // 添加這行
    });

    connect(adminwin, &adminwindow::statuschange, this, [=](){
        adminwin->hide();           // 隱藏管理員視窗
        ssw->show();                // 顯示
        resetSessionTimer(); // 添加這行
    });

    // 連接帳戶列表視窗的登出與返回訊號
    connect(acclist, &acclistWindow::logout, this, [=](){
        acclist->hide();            // 隱藏帳戶列表視窗
        loginWin->clearPassword();  // 清除密碼欄位
        loginWin->show();           // 顯示登入視窗
        sessionActive = false;      // 添加這行
        sessionTimer->stop();       // 添加這行
    });

    connect(acclist, &acclistWindow::back, this, [=](){
        acclist->hide();            // 隱藏帳戶列表視窗
        adminwin->show();           // 顯示管理員視窗
        resetSessionTimer(); // 添加這行
    });

    // 連接新增帳戶視窗的登出與返回訊號
    connect(addacc, &addaccwindow::logout, this, [=](){
        addacc->hide();             // 隱藏新增帳戶視窗
        loginWin->clearPassword();  // 清除密碼欄位
        loginWin->show();           // 顯示登入視窗
        sessionActive = false;      // 添加這行
        sessionTimer->stop();       // 添加這行
    });

    connect(addacc, &addaccwindow::back, this, [=](){
        addacc->hide();             // 隱藏新增帳戶視窗
        adminwin->show();           // 顯示管理員視窗
        resetSessionTimer(); // 添加這行
    });

    // 連接密碼設定視窗的登出與返回訊號
    connect(spw, &setpasswordwindow::logout, this, [=](){
        spw->hide();                // 隱藏密碼設定視窗
        loginWin->clearPassword();  // 清除密碼欄位
        loginWin->show();           // 顯示登入視窗
        sessionActive = false;      // 添加這行
        sessionTimer->stop();       // 添加這行
    });

    connect(spw, &setpasswordwindow::back, this, [=](){
        spw->hide();                // 隱藏密碼設定視窗
        adminwin->show();           // 顯示管理員視窗
        resetSessionTimer(); // 添加這行
    });


    connect(ssw, &setstatuswindow::logout, this, [=](){
        ssw->hide();
        loginWin->clearPassword();  // 清除密碼欄位
        loginWin->show();           // 顯示登入視窗
        sessionActive = false;      // 添加這行
        sessionTimer->stop();       // 添加這行
    });    
    
    connect(ssw, &setstatuswindow::back, this, [=](){
        ssw->hide();                // 隱藏
        adminwin->show();           // 顯示管理員視窗
        resetSessionTimer(); // 添加這行
    });

    // 連接變更密碼視窗的登出與返回訊號
    connect(changepw, &changepasswordWindow::logout, this, [=](){
        changepw->hide();           // 隱藏變更密碼視窗
        loginWin->clearPassword();  // 清除密碼欄位
        loginWin->show();           // 顯示登入視窗
        sessionActive = false;      // 添加這行
        sessionTimer->stop();       // 添加這行
    });

    connect(changepw, &changepasswordWindow::back, this, [=](){
        changepw->hide();           // 隱藏變更密碼視窗
        userWin->setWelcomeText(Controller::Getaccount()); // 新增：顯示歡迎訊息
        userWin->show();            // 顯示使用者視窗
        resetSessionTimer(); // 添加這行
    });    // 連接三個交易視窗的 showSummary signal
    connect(withdrowwin, &withdrowWindow::showSummary, this, [=](const QString &type, int amount, const QString &target, double balance){
        withdrowwin->hide();
        summaryWin->setSummary(type, amount, target, balance);
        summaryWin->show();
        resetSessionTimer(); // 添加這行

    });
    connect(depositwin, &depositWindow::showSummary, this, [=](const QString &type, int amount, const QString &target, double balance){
        depositwin->hide();
        summaryWin->setSummary(type, amount, target, balance);
        summaryWin->show();
        resetSessionTimer(); // 添加這行
    });
    connect(transwin, &TransWindow::showSummary, this, [=](const QString &type, int amount, const QString &target, double balance){
        transwin->hide();
        summaryWin->setSummary(type, amount, target, balance);
        summaryWin->show();
        resetSessionTimer(); // 添加這行

    });

    // 連接 summarywindow 的按鈕 signals
    connect(summaryWin, &summarywindow::back, this, [=](){
        summaryWin->hide();
        userWin->setWelcomeText(Controller::Getaccount());
        userWin->show();
        resetSessionTimer(); // 添加這行
    });
    connect(summaryWin, &summarywindow::logout, this, [=](){
        summaryWin->hide();
        Controller::Setaccount("");
        Controller::Setid("");
        loginWin->clearPassword();
        loginWin->show();
        sessionActive = false;      // 添加這行
        sessionTimer->stop();       // 添加這行
    });


    
    sessionTimer = new QTimer(this);
    sessionTimer->setSingleShot(true);
    sessionTimer->setInterval(90000); // 90秒 = 1分30秒
    sessionActive = false;
    
    // 連接會話超時信號
    connect(sessionTimer, &QTimer::timeout, this, &Controller::onSessionTimeout);

}

// === 會話超時方法實作 ===
void Controller::resetSessionTimer()
{
    if (!sessionActive) return; // 未登入時不重置
    
    sessionTimer->stop();
    sessionTimer->start();
}

void Controller::handleSessionTimeout()
{
    sessionActive = false;
    sessionTimer->stop();
    
    // 清除會話資料
    Controller::Setaccount("");
    Controller::Setid("");
    
    // 隱藏所有視窗
    userWin->hide();
    adminwin->hide();
    depositwin->hide();
    withdrowwin->hide();
    transwin->hide();
    docuwin->hide();
    changepw->hide();
    summaryWin->hide();
    acclist->hide();
    addacc->hide();
    spw->hide();
    ssw->hide();
    
    // 顯示登入視窗
    loginWin->clearPassword();
    loginWin->show();
}

void Controller::onSessionTimeout()
{
    QMessageBox::information(nullptr, "會話超時", 
                            "由於長時間無操作，系統已自動登出。\n請重新登入。");
    handleSessionTimeout();
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
 * @brief 取得餘額資訊
 * @return 目前設定的餘額數值
 */
double Controller::Getbalance() {
    return balance;
}

/**
 * @brief 啟動整個系統
 * 
 * 呼叫此方法來顯示登入畫面，開始程式流程
 */
void Controller::start() {
    loginWin->show();
}
