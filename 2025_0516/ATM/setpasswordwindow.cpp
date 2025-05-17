#include "setpasswordwindow.h"
#include "ui_setpasswordwindow.h"
#include "databasemanager.h"
#include <QMessageBox>
#include <QCryptographicHash>
#include <QSqlQuery>
#include <QSqlError>

setpasswordwindow::setpasswordwindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::setpasswordwindow)
{
    ui->setupUi(this);
    ui->accountEdit->setFocus();

     connect(ui->accountEdit, &QLineEdit::returnPressed,ui->passwordEdit, QOverload<>::of(&QWidget::setFocus));
}

setpasswordwindow::~setpasswordwindow()
{
    delete ui;
}



void setpasswordwindow::on_back_clicked(){
    qDebug()<<"on_back_clicked()";
    emit back();
}
void setpasswordwindow::on_logout_clicked(){
    qDebug()<<"on_logout_clicked()";
    emit logout();
}

void setpasswordwindow::on_clear_clicked(){
    qDebug()<<"on_clear_clicked()";
    ui->accountEdit->clear();
    ui->passwordEdit->clear();
}

void setpasswordwindow::on_change_clicked(){
    qDebug()<<"on_change_clicked()";


    QString account = ui->accountEdit->text().trimmed();
    QString pass = ui->passwordEdit->text();
    if(account.isEmpty()||pass.isEmpty()){
        QMessageBox::warning(this,"ERROR","欄位未填寫完畢");
        return;
    }

    //指定連線名稱
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
    // 使用 SHA256 加密密碼
    QString hashedPass = QCryptographicHash::hash(pass.toUtf8(), QCryptographicHash::Sha256).toHex();


    query.prepare("UPDATE users SET password = (?) WHERE id = (?)");
    query.addBindValue(hashedPass);
    query.addBindValue(userId);


    if (!query.exec()) {
        QMessageBox::critical(this, "Update Error", "更新密碼失敗！");
        qDebug() << "[DEBUG] Update error:" << query.lastError().text();
        return;
    }

    ui->accountEdit->clear();
    ui->passwordEdit->clear();
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Success");
    msgBox.setText("更新成功\n請告知客戶新密碼並提醒即早更換");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStyleSheet(
        "QMessageBox {"
        "   background-color: #f5f5f5;" // 淺灰白底
        "   font: 14px 'Arial';"
        "}"
        "QLabel {"
        "   background-color: #f5f5f5;" // 淺灰白底
        "   color: #ff0000;" // 紅字
        "}"
        "QPushButton {"
        "   background-color: #332d2d;" // 綠色按鈕
        "   color: white;"
        "   border-radius: 5px;"
        "   padding: 5px 15px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #45a049;" // 懸停略暗
        "}"
        );
    msgBox.exec();


}

