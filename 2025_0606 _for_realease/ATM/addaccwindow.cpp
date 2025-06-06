#include "addaccwindow.h"
#include "ui_addaccwindow.h"
#include "databasemanager.h"
#include <QMessageBox>
#include <QCryptographicHash>
#include <QSqlQuery>
#include <QSqlError>

addaccwindow::addaccwindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::addaccwindow)
{
    ui->setupUi(this);
    // 設置焦點到帳號輸入框
    ui->accountEdit->setFocus();




    connect(ui->accountEdit, &QLineEdit::returnPressed,ui->passwordEdit, QOverload<>::of(&QWidget::setFocus));
    connect(ui->passwordEdit, &QLineEdit::returnPressed,ui->passwordEdit2, QOverload<>::of(&QWidget::setFocus));
    connect(ui->passwordEdit2, &QLineEdit::returnPressed,ui->balanceEdit, QOverload<>::of(&QWidget::setFocus));
    connect(ui->balanceEdit, &QLineEdit::returnPressed,ui->StatusCombo, QOverload<>::of(&QWidget::setFocus));
}

addaccwindow::~addaccwindow()
{
    delete ui;
}
void addaccwindow::on_back_clicked(){
    qDebug()<<"addaccwindow::on_back_clicked()";
    emit back();
}

void addaccwindow::on_logout_clicked(){
    qDebug()<<"addaccwindow::on_logout_clicked()";
    emit logout();
}

void addaccwindow::on_clear_clicked(){
    qDebug()<<"addaccwindow::on_clear_clicked()";
    ui->accountEdit->clear();
    ui->passwordEdit->clear();
    ui->passwordEdit2->clear();
    ui->balanceEdit->clear();
    ui->StatusCombo->setCurrentIndex(0);
}

void addaccwindow::on_add_clicked() {
    qDebug() << "on_add_clicked()";

    QString account = ui->accountEdit->text().trimmed();
    QString password = ui->passwordEdit->text();
    QString password2 = ui->passwordEdit2->text();
    QString balanceStr = ui->balanceEdit->text();
    QString statusText = ui->StatusCombo->currentText();

    if (account.isEmpty() || password.isEmpty() || password2.isEmpty() || balanceStr.isEmpty() || statusText == "請選擇用戶屬性") {
        QMessageBox::warning(this, "Input Error", "請填寫所有欄位。");
        return;
    }

    if (password != password2) {
        QMessageBox::warning(this, "Password Error", "兩次輸入的密碼不一致。");
        return;
    }

    bool ok;
    double balance = balanceStr.toDouble(&ok);
    if (!ok || balance < 0) {
        QMessageBox::warning(this, "Balance Error", "請輸入有效的餘額數值（大於等於 0）。");
        return;
    }

    int status = 0;
    if (statusText == "普通用戶") status = 0;
    else if (statusText == "管理人員") status = 500;

    QByteArray hashedPassword = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();

    // ✅ 正確使用 DatabaseManager 和指定連線名稱
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

    QSqlQuery query(db);  // ✅ 指定 connection
    query.prepare("INSERT INTO users (account, password, status, balance) VALUES (?, ?, ?, ?)");
    query.addBindValue(account);
    query.addBindValue(QString(hashedPassword));
    query.addBindValue(status);
    query.addBindValue(balance);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "新增帳號失敗，帳號可能已存在。");
        qDebug() << "[DEBUG] Insert failed:" << query.lastError().text();
        return;
    }

    QMessageBox::information(this, "Success", "新增帳號成功！");
    ui->accountEdit->clear();
    ui->passwordEdit->clear();
    ui->passwordEdit2->clear();
    ui->balanceEdit->clear();
    ui->StatusCombo->setCurrentIndex(0);
}
