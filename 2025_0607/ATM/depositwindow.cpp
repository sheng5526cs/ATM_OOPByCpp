#include "depositwindow.h"
#include "ui_depositwindow.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include "controller.h"
#include "databasemanager.h"

depositWindow::depositWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::depositWindow)
{
    ui->setupUi(this);
    refreshBalance();
    connect(ui->clear, &QPushButton::clicked, this, &depositWindow::on_clear_clicked);
    connect(ui->deposit_btn, &QPushButton::clicked, this, &depositWindow::on_deposit_btn_clicked);
    connect(ui->back, &QPushButton::clicked, this, &depositWindow::on_back_clicked);
    connect(ui->logout, &QPushButton::clicked, this, &depositWindow::on_logout_clicked);
}

depositWindow::~depositWindow()
{
    delete ui;
}

void depositWindow::refreshBalance() {
    double balance = Controller::Getbalance();
    ui->label_balance->setText(QString("目 前 餘 額 : %1 元").arg(balance, 0, 'f', 0));
}

void depositWindow::on_clear_clicked() {
    ui->deposit->clear();
}

void depositWindow::on_deposit_btn_clicked() {
    // 防止 QMessageBox 跳兩次：只保留一種連接方式
    disconnect(ui->deposit_btn, nullptr, this, nullptr); // 先斷開所有連接
    bool ok = false;
    int amount = ui->deposit->text().toInt(&ok);
    if (!ok || amount <= 0) {
        QMessageBox::warning(this, "錯誤", "請輸入正確的存款金額。");
        connect(ui->deposit_btn, &QPushButton::clicked, this, &depositWindow::on_deposit_btn_clicked); // 恢復連接
        ui->deposit->clear();
        return;
    }
    if (!dbm.connect()) {
        QMessageBox::critical(this, "Database Error", "無法連線至系統。");
        connect(ui->deposit_btn, &QPushButton::clicked, this, &depositWindow::on_deposit_btn_clicked);
        ui->deposit->clear();
        return;
    }
    QSqlDatabase db = QSqlDatabase::database("atm_connection");
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Database Error", "系統未開啟！");
        connect(ui->deposit_btn, &QPushButton::clicked, this, &depositWindow::on_deposit_btn_clicked);
        ui->deposit->clear();
        return;
    }
    QSqlQuery balQuery(db);
    balQuery.prepare("SELECT balance FROM users WHERE account = ?");
    balQuery.addBindValue(Controller::Getaccount());
    if (!balQuery.exec() || !balQuery.next()) {
        QMessageBox::critical(this, "錯誤", "查詢餘額失敗");
        connect(ui->deposit_btn, &QPushButton::clicked, this, &depositWindow::on_deposit_btn_clicked);
        ui->deposit->clear();
        return;
    }
    double balance = balQuery.value(0).toDouble();
    double newBalance = balance + amount;
    QString now = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QSqlQuery oldQuery(db);
    oldQuery.prepare("SELECT change_amount_1, balance_1, change_time_1, change_amount_2, balance_2, change_time_2, change_amount_3, balance_3, change_time_3, change_amount_4, balance_4, change_time_4 FROM users WHERE account = ?");
    oldQuery.addBindValue(Controller::Getaccount());
    if (!oldQuery.exec() || !oldQuery.next()) {
        QMessageBox::critical(this, "錯誤", "查詢交易紀錄失敗");
        connect(ui->deposit_btn, &QPushButton::clicked, this, &depositWindow::on_deposit_btn_clicked);
        ui->deposit->clear();
        return;
    }
    QSqlQuery query(db);
    query.prepare(R"(
        UPDATE users SET
            balance = ?,
            change_amount_5 = change_amount_4, balance_5 = balance_4, change_time_5 = change_time_4,
            change_amount_4 = change_amount_3, balance_4 = balance_3, change_time_4 = change_time_3,
            change_amount_3 = change_amount_2, balance_3 = balance_2, change_time_3 = change_time_2,
            change_amount_2 = change_amount_1, balance_2 = balance_1, change_time_2 = change_time_1,
            change_amount_1 = ?, balance_1 = ?, change_time_1 = ?
        WHERE account = ?
    )");
    query.addBindValue(newBalance);
    query.addBindValue(amount);
    query.addBindValue(newBalance);
    query.addBindValue(now);
    query.addBindValue(Controller::Getaccount());
    if (!query.exec()) {
        QMessageBox::critical(this, "資料庫錯誤", "存款失敗：" + query.lastError().text());
        connect(ui->deposit_btn, &QPushButton::clicked, this, &depositWindow::on_deposit_btn_clicked);
        ui->deposit->clear();
        return;
    }
    // 存款成功後 emit showSummary
    Controller::Setbalance(newBalance);
    refreshBalance();
    emit showSummary("存款", amount, "", newBalance);
    ui->deposit->clear();
    connect(ui->deposit_btn, &QPushButton::clicked, this, &depositWindow::on_deposit_btn_clicked); // 恢復連接
}

void depositWindow::on_back_clicked() { emit back(); }
void depositWindow::on_logout_clicked() { emit logout(); }
