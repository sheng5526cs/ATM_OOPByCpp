#include "adminwindow.h"
#include "ui_adminwindow.h"
#include <qmessagebox.h>
adminwindow::adminwindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::adminwindow)
{
    ui->setupUi(this);
}

adminwindow::~adminwindow()
{
    delete ui;
}

void adminwindow::on_logout_clicked(){
    qDebug() << "click logout";
    emit logout();
}


void adminwindow::on_AccountlistButton_clicked(){
    qDebug() << "AccountlistButton clicked";
    emit Accountlist();
}

void adminwindow::on_AddAccountButton_clicked(){
    qDebug() << "AddAccountButton clicked";
    emit addaccountwindow();
}



void adminwindow::on_ResetpasswordButton_clicked(){
    qDebug() << "ResetpasswordButton clicked";
    emit passwordsetting();
}


void adminwindow::on_ChangestatusButton_clicked(){
    qDebug() << "ChangestatusButton clicked";
    emit statuschange();
}


void adminwindow::on_aOtherBotton_clicked(){
    qDebug() << "aOtherBottonButton clicked";

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("聯繫開發者");
    msgBox.setText("更多軟體問題或操作請聯繫開發者：\n\n"
                   "姓名：吳維盛\n"
                   "電子郵件：B3230665@ulive.pccu.edu.tw\n"
                   "GitHub：github.com/sheng5526cs\n"
                   "造成不便之處，敬請見諒"

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
