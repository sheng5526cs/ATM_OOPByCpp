#include "userwindow.h"
#include "ui_userwindow.h"
#include <QDebug>
#include <QMessageBox>

userwindow::userwindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::userwindow)
{
    ui->setupUi(this);
}

userwindow::~userwindow()
{
    delete ui;
}

void userwindow::on_logout_clicked(){
    qDebug() << "click logout";
    emit logout();
}

void userwindow::on_changepass_clicked(){
    qDebug() << "click changepass";
    emit changepass();
}

void userwindow::on_deposit_clicked(){
    qDebug() << "click deposit";
    emit deposit();
}

void userwindow::on_withdrow_clicked(){
    qDebug() << "click withdraw";
    emit withdraw();
}

void userwindow::on_trans_clicked(){
    qDebug() << "click transfer";
    emit transfer();
}

void userwindow::on_docu_clicked(){
    qDebug() << "click transaction history";
    emit transactionHistory();
}

void userwindow::on_idk_clicked(){
    qDebug() << "click contact service";
    
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("聯繫客服");
    msgBox.setText("如需協助或有任何問題，請聯繫我們的客服團隊：\n\n"
                   "客服專線：(02) 2700-8888\n"
                   "服務時間：週一至週五 09:00-18:00\n"
                   "電子郵件：service@atmbank.com.tw\n"
                   "線上客服：www.atmbank.com.tw/chat\n\n"
                   "感謝您的使用，我們將竭誠為您服務！"
                   );

    // 白底黑字 + 專業字體 + 按鈕樣式
    msgBox.setStyleSheet(
        "QMessageBox { background-color: white; }"
        "QLabel {background-color: white; color: black; font-family: 'Microsoft YaHei','Noto Sans CJK TC','PingFang TC','Arial'; font-size: 16px; }"
        "QPushButton { background-color: #f0f0f0; color: black; border: 1px solid #999; padding: 6px 12px; min-width: 80px; }"
        "QPushButton:hover { background-color: #e0e0e0; }"
        );

    // 設定大小
    msgBox.setMinimumSize(480, 240);

    // 顯示對話框
    msgBox.exec();
    
    emit contactService();
}
