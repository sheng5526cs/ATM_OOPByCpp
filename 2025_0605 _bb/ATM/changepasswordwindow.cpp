#include "changepasswordwindow.h"
#include "ui_changepasswordwindow.h"
#include "controller.h"
#include "databasemanager.h"
#include <QMessageBox>
#include <QCryptographicHash>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

changepasswordWindow::changepasswordWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::changepasswordWindow)
{
    ui->setupUi(this);
    
    // 設定密碼輸入框為密碼模式
    ui->nowpass->setEchoMode(QLineEdit::Password);
    ui->newpass->setEchoMode(QLineEdit::Password);
    ui->new2->setEchoMode(QLineEdit::Password);
    
    // 設定焦點順序
    ui->nowpass->setFocus();
    
    // 設定 Enter 鍵切換焦點
    connect(ui->nowpass, &QLineEdit::returnPressed, ui->newpass, QOverload<>::of(&QWidget::setFocus));
    connect(ui->newpass, &QLineEdit::returnPressed, ui->new2, QOverload<>::of(&QWidget::setFocus));
    connect(ui->new2, &QLineEdit::returnPressed, this, &changepasswordWindow::on_check_clicked);
}

changepasswordWindow::~changepasswordWindow()
{
    delete ui;
}

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            void changepasswordWindow::on_back_clicked()
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            {
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                qDebug() << "on_back_clicked()";
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                emit back();
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            }

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            void changepasswordWindow::on_logout_clicked()
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            {
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                qDebug() << "on_logout_clicked()";
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                emit logout();
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            }

void changepasswordWindow::on_clear_clicked()
{
    qDebug() << "on_clear_clicked()";
    ui->nowpass->clear();
    ui->newpass->clear();
    ui->new2->clear();
    ui->nowpass->setFocus();
}

void changepasswordWindow::on_check_clicked()
{
    qDebug() << "on_check_clicked()";
    
    QString currentPass = ui->nowpass->text();
    QString newPass = ui->newpass->text();
    QString confirmPass = ui->new2->text();
    
    // 檢查是否有空白欄位
    if (currentPass.isEmpty() || newPass.isEmpty() || confirmPass.isEmpty()) {
        QMessageBox::warning(this, "ERROR", "所有欄位都必須填寫完畢");
        return;
    }
    
    // 檢查新密碼是否一致
    if (newPass != confirmPass) {
        QMessageBox::warning(this, "ERROR", "新密碼與確認密碼不一致");
        ui->newpass->clear();
        ui->new2->clear();
        ui->newpass->setFocus();
        return;
    }
    
    // 檢查新密碼長度 (建議至少4位)
    if (newPass.length() < 4) {
        QMessageBox::warning(this, "ERROR", "新密碼長度至少需要4位數");
        ui->newpass->clear();
        ui->new2->clear();
        ui->newpass->setFocus();
        return;
    }
    
    // 檢查新密碼不能與舊密碼相同
    if (currentPass == newPass) {
        QMessageBox::warning(this, "ERROR", "新密碼不能與目前密碼相同");
        ui->newpass->clear();
        ui->new2->clear();
        ui->newpass->setFocus();
        return;
    }
    
    // 取得當前用戶 ID
    QString currentUserId = Controller::Getid();
    if (currentUserId.isEmpty()) {
        QMessageBox::critical(this, "ERROR", "無法取得用戶資訊，請重新登入");
        return;
    }
    
    // 連接資料庫 - 使用與 setpasswordwindow.cpp 相同的方式
    DatabaseManager dbm;
    if (!dbm.connect()) {
        QMessageBox::critical(this, "Database Error", "無法連線至系統。");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database("atm_connection");
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Database Error", "系統未開啟！");
        qDebug() << "[DEBUG] Database is NOT open in changepasswordwindow:" << db.lastError().text();
        return;
    }

    qDebug() << "Database connected successfully.";
    
    // 使用 SHA256 加密目前輸入的密碼
    QString hashedCurrentPass = QCryptographicHash::hash(currentPass.toUtf8(), QCryptographicHash::Sha256).toHex();
    
    QSqlQuery query(db);
    // 驗證目前密碼是否正確
    query.prepare("SELECT id FROM users WHERE id = ? AND password = ?");
    query.addBindValue(currentUserId);
    query.addBindValue(hashedCurrentPass);
    
    if (!query.exec()) {
        QMessageBox::critical(this, "Query Error", "驗證密碼失敗！");
        qDebug() << "[DEBUG] Query error:" << query.lastError().text();
        return;
    }
    
    if (!query.next()) {
        QMessageBox::critical(this, "ERROR", "目前密碼不正確！");
        ui->nowpass->clear();
        ui->nowpass->setFocus();
        return;
    }
    
    // 使用 SHA256 加密新密碼
    QString hashedNewPass = QCryptographicHash::hash(newPass.toUtf8(), QCryptographicHash::Sha256).toHex();
    
    // 更新密碼
    query.prepare("UPDATE users SET password = ? WHERE id = ?");
    query.addBindValue(hashedNewPass);
    query.addBindValue(currentUserId);
    
    if (!query.exec()) {
        QMessageBox::critical(this, "Update Error", "更新密碼失敗！");
        qDebug() << "[DEBUG] Update error:" << query.lastError().text();
        return;
    }
    
    // 清空所有輸入框
    ui->nowpass->clear();
    ui->newpass->clear();
    ui->new2->clear();
    
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Success");
    msgBox.setText("密碼更新成功！\n請記住您的新密碼");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStyleSheet(
        "QMessageBox {"
        "   background-color: #f5f5f5;"
        "   font: 14px 'Arial';"
        "}"
        "QLabel {"
        "   background-color: #f5f5f5;"
        "   color: #28a745;"
        "}"
        "QPushButton {"
        "   background-color: #28a745;"
        "   color: white;"
        "   border-radius: 5px;"
        "   padding: 5px 15px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #218838;"
        "}"
    );
    msgBox.exec();
}
