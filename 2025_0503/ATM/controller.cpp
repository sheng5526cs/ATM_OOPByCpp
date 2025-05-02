#include "controller.h"
#include "loginwindow.h"  // 需要具體實現的頭文件
#include "userwindow.h"   // 需要具體實現的頭文件
#include "DatabaseManager.h"
#include "adminwindow.h"
#include <QMessageBox> // 引入訊息框類

QString Controller::id = "";
QString Controller::account = "";
double Controller::balance = 0.0;



Controller::Controller(QObject *parent)
    : QObject(parent)
{
    loginWin = new loginWindow;  // 在這裡使用具體類別
    userWin = new userwindow;    // 在這裡使用具體類別
    adminwin = new adminwindow;


    if (!dbm.connect()) {
        QMessageBox::critical(nullptr, "Database Error", "Failed to connect to the database.");
        return;
    }


    // 連接登入成功訊號，傳送帳戶資訊並更新靜態變數
    connect(loginWin, &loginWindow::loginPress, this, [=](const QString &account, const QString &password,const bool &isadmin) {
        qDebug() << "Controller received  Account:" << account << "Password:" << password;

        bool Login=false;
        if(isadmin){
            Login = (account == "admin" && password == "5678");
            if(Login){
                loginWin->hide();
                adminwin->show();
            }
            else{
                QMessageBox msgBox(loginWin); // 創建訊息框
                msgBox.setWindowTitle("Error"); // 設定標題
                msgBox.setText("Invalid account or password!"); // 設定訊息
                msgBox.setStandardButtons(QMessageBox::Ok); // 設定 OK 按鈕
                msgBox.exec(); // 模態執行
            }
        }
        else if(account == "user"){
            Login = (account == "user" && password == "1234");
            if(Login){
                qDebug() << "Login successful! Account:" << account << "Password:" << password;
                // 設置靜態帳戶資訊
                Controller::Setaccount(account);
                Controller::Setid(password);      // 設置靜態密碼資訊（如果需要的話）

                // 檢查靜態變數是否設置正確
                qDebug() << "Static Account set to:" << Controller::Getaccount();
                qDebug() << "Static ID set to:" << Controller::Getid();

                loginWin->hide();
                userWin->show();
            }
            else{
                QMessageBox msgBox(loginWin); // 創建訊息框
                msgBox.setWindowTitle("Error"); // 設定標題
                msgBox.setText("Invalid account or password!"); // 設定訊息
                msgBox.setStandardButtons(QMessageBox::Ok); // 設定 OK 按鈕
                msgBox.exec(); // 模態執行
            }

        }
        else{
            QString password_select = dbm.fetchPassword(account);
            qDebug() << "Login successful! Account:" << account << "Password:" << password_select;
            if(password==password_select){
                loginWin->hide();
                userWin->show();
            }
            else{
                QMessageBox msgBox(loginWin); // 創建訊息框
                msgBox.setWindowTitle("Error"); // 設定標題
                msgBox.setText("Invalid account or password!"); // 設定訊息
                msgBox.setStandardButtons(QMessageBox::Ok); // 設定 OK 按鈕
                msgBox.exec(); // 模態執行
            }

        }





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

    connect(adminwin, &adminwindow::logout, this, [=](){
        adminwin->hide();
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
