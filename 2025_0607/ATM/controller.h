#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "databasemanager.h"
#include "depositwindow.h"
#include "summarywindow.h" // 包含 summarywindow 的標頭檔
#include <QObject>
#include <QTimer> // 新增：會話超時計時器
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
    TransWindow *transwin ; // 新增：轉帳視窗
    summarywindow *summaryWin = nullptr; // 新增 summarywindow 指標    static QString id;      // 靜態變量
    static QString account;
    static double balance;

    DatabaseManager dbm;
    
    // 會話超時管理
    QTimer *sessionTimer;          // 會話超時計時器
    QTimer *warningTimer;          // 警告提示計時器
    static const int SESSION_TIMEOUT = 90000;  // 1.5分鐘 = 90秒 = 90000毫秒
    static const int WARNING_TIME = 75000;     // 1分15秒警告
    
private slots:
    void resetSessionTimer();      // 重置會話計時器
    void showSessionWarning();     // 顯示會話即將超時警告
    void sessionTimeout();         // 會話超時處理
};

#endif // CONTROLLER_H
