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
                   "客服專線：(02) 0000-8888\n"
                   "服務時間：週一至週五 09:00-18:00\n"
                   "電子郵件：B3208660@ulive.pccu.edu.tw\n"
                   "電子郵件：B3230665@ulive.pccu.edu.tw\n"
                   "線上客服：https://www.instagram.com/__ruu.23\n"
                   "https://www.instagram.com/wwu_.05.26\n\n"
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
    

}

void userwindow::setWelcomeText(const QString &name) {
    // 主標題：登入成功
    QString html = "<span style='font-size:26pt;font-weight:bold;color:#fff;'>登　入　成　功　　！</span>";
    ui->label->setText(html);
    ui->label->setAlignment(Qt::AlignCenter);
    ui->label->setStyleSheet("QLabel { font: bold 26pt 'Microsoft YaHei UI'; color: #fff; background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #667eea, stop:1 #764ba2); border-radius: 0px; padding: 15px; margin: 0px; }");
    // 副標題：歡迎使用者 XXX
    QString html2 = QString("<span style='font-size:20pt;font-weight:bold;color:#ffe066;text-shadow:1px 1px 4px #333;'>歡 迎 使 用 者 <b>%1</b></span>").arg(name);
    ui->label_2->setText(html2);
    ui->label_2->setAlignment(Qt::AlignCenter);
    ui->label_2->setStyleSheet("QLabel { font: bold 20pt 'Microsoft YaHei UI'; color: #ffe066; background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #667eea, stop:1 #764ba2); border-radius: 0px; padding: 10px; margin: 0px; }");
}
