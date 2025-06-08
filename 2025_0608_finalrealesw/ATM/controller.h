#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "databasemanager.h"
#include "depositwindow.h"
#include "summarywindow.h" 
#include <QObject>
#include <QTimer>

// 前向宣告 loginWindow 和 userwindow 類別
class loginWindow;
class userwindow;
class adminwindow;
class acclistWindow;
class addaccwindow;
class setpasswordwindow;
class setstatuswindow;
class changepasswordWindow;
class withdrowWindow; // 前向宣告 withdrowWindow
class docuWindow; // 前向宣告 docuWindow
class depositWindow;
class TransWindow; // 前向宣告
class PPTwindow; // 前向宣告 PPTwindow
class ThanksForListeningWindow; // 前向宣告 ThanksForListeningWindow

class Controller : public QObject
{
    Q_OBJECT

public:
    explicit Controller(QObject *parent = nullptr);
    void start(); // 啟動整個流程（例如在 main 裡叫這個）
    
    // 靜態的 getter 和 setter 函數
    static QString Getid();
    static QString Getaccount();
    static double Getbalance();

    static void Setid(const QString &id);
    static void Setaccount(const QString &account);
    static void Setbalance(double balance);
    
    // 會話管理函數
    void resetSessionTimer();    // 重置會話計時器
    void handleSessionTimeout(); // 處理會話超時

private slots:
    void onSessionTimeout();     // 會話超時槽函數

private:
    loginWindow *loginWin;  // 登入畫面物件
    userwindow *userWin;    // 使用者畫面物件
    adminwindow *adminwin;
    acclistWindow *acclist;
    addaccwindow *addacc;
    setpasswordwindow *spw;
    setstatuswindow *ssw;
    changepasswordWindow *changepw;
    withdrowWindow *withdrowwin; // 新增：提款視窗
    docuWindow *docuwin; // 新增：交易紀錄視窗
    depositWindow *depositwin; // 新增：存款視窗
    TransWindow *transwin; // 新增：轉帳視窗
    summarywindow *summaryWin; // 新增 summarywindow 指標
    PPTwindow *pptWin; // 新增：PPT 視窗
    ThanksForListeningWindow *thanksWin; // 新增：感謝收聽視窗

    // 會話超時計時器
    QTimer *sessionTimer;       // 會話計時器 (90秒)
    bool sessionActive;         // 會話狀態標記

    static QString id;      // 靜態變量
    static QString account;
    static double balance;

    DatabaseManager dbm;
};

#endif // CONTROLLER_H
