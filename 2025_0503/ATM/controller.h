#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "databasemanager.h"
#include <QObject>
// 前向宣告 loginWindow 和 userwindow 類別
class loginWindow;
class userwindow;
class adminwindow;

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

    static QString id;      // 靜態變量
    static QString account;
    static double balance;
    DatabaseManager dbm;
};

#endif // CONTROLLER_H
