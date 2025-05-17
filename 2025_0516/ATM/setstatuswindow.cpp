#include "setstatuswindow.h"
#include "ui_setstatuswindow.h"
#include "databasemanager.h"
#include <QMessageBox>
#include <QCryptographicHash>
#include <QSqlQuery>
#include <QSqlError>
setstatuswindow::setstatuswindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::setstatuswindow)
{
    ui->setupUi(this);
}

setstatuswindow::~setstatuswindow()
{
    delete ui;
}


void setstatuswindow::on_back_clicked(){
    qDebug()<<"on_back_clicked()";
    emit back();
}
void setstatuswindow::on_logout_clicked(){
    qDebug()<<"on_logout_clicked()";
    emit logout();
}

void setstatuswindow::on_clear_clicked(){
    qDebug()<<"on_clear_clicked()";
    ui->accountEdit->clear();
    ui->statuscomboBox->setCurrentIndex(0);
}

void setstatuswindow::on_change_clicked(){
    qDebug()<<"on_change_clicked()";
    QString account = ui->accountEdit->text().trimmed();
    QString statusText = ui->statuscomboBox->currentText();
    if (account.isEmpty() ||  statusText == "選擇狀態......"){
        QMessageBox::warning(this, "ERROR","欄位未填寫完畢");
        return;
    }
    int status = 0;
    if (statusText == "普通用戶") status = 0;
    else if (statusText == "管理人員") status = 500;
    else if (statusText == "上鎖(普通用戶)") status = 3;
    else if (statusText == "解鎖(普通用戶)") status = 0;
    DatabaseManager dbm;
    if (!dbm.connect()) {
        QMessageBox::critical(this, "Database Error", "無法連線至系統。");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database("atm_connection");
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Database Error", "系統未開啟！");
        qDebug() << "[DEBUG] Database is NOT open in addaccwindow:" << db.lastError().text();
        return;
    }

    qDebug() << "Database connected successfully.";

    QSqlQuery query(db);
    query.prepare("SELECT id FROM users WHERE account = ?");
    query.addBindValue(account);

    if (!query.exec()) {
        QMessageBox::critical(this, "Query Error", "查詢帳號失敗！");
        qDebug() << "[DEBUG] Query error:" << query.lastError().text();
        return;
    }

    if (!query.next()) {
        QMessageBox::critical(this, "ERROR", "帳號不存在！");
        return;
    }


    int userId = query.value("id").toInt();
    query.prepare("UPDATE users SET status = (?) WHERE id = (?)");
    query.addBindValue(status);
    query.addBindValue(userId);
    if (!query.exec()) {
        QMessageBox::critical(this, "Update Error", "更新密碼失敗！");
        qDebug() << "[DEBUG] Update error:" << query.lastError().text();
        return;
    }
    QMessageBox::information(this, "Success", "帳號狀態更改成功！");
    ui->accountEdit->clear();
    ui->statuscomboBox->setCurrentIndex(0);
}

