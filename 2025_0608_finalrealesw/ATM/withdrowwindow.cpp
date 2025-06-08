#include "withdrowwindow.h"
#include "ui_withdrowwindow.h"
#include "controller.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

withdrowWindow::withdrowWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::withdrowWindow)
{
    ui->setupUi(this);
    refreshBalance();

}

withdrowWindow::~withdrowWindow()
{
    delete ui;
}

void withdrowWindow::refreshBalance() {
    double balance = Controller::Getbalance();
    ui->label_4->setText(QString("目 前 餘 額 : %1 元").arg(balance, 0, 'f', 0));
}

void withdrowWindow::withdraw(int amount){
    bool ok = false;
    // 連線資料庫（比照 addaccwindow）
    DatabaseManager dbm;
    if (!dbm.connect()) {
        QMessageBox::critical(this, "Database Error", "無法連線至系統。");
        ui->withdrow->clear();
        return;
    }
    QSqlDatabase db = QSqlDatabase::database("atm_connection");
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Database Error", "系統未開啟！");
        ui->withdrow->clear();
        return;
    }
    // 查詢目前餘額
    QSqlQuery balQuery(db);
    balQuery.prepare("SELECT balance FROM users WHERE account = ?");
    balQuery.addBindValue(Controller::Getaccount());
    if (!balQuery.exec() || !balQuery.next()) {
        QMessageBox::critical(this, "錯誤", "查詢餘額失敗");
        ui->withdrow->clear();
        return;
    }
    double balance = balQuery.value(0).toDouble();
    if (amount > balance) {
        QMessageBox::warning(this, "餘額不足", "您的餘額不足。");
        ui->withdrow->clear();
        return;
    }
    // 取得舊的 1~4 筆交易紀錄
    QSqlQuery oldQuery(db);
    oldQuery.prepare("SELECT change_amount_1, balance_1, change_time_1, change_amount_2, balance_2, change_time_2, change_amount_3, balance_3, change_time_3, change_amount_4, balance_4, change_time_4 FROM users WHERE account = ?");
    oldQuery.addBindValue(Controller::Getaccount());
    if (!oldQuery.exec() || !oldQuery.next()) {
        QMessageBox::critical(this, "錯誤", "查詢交易紀錄失敗");
        ui->withdrow->clear();
        return;
    }
    // 取得新餘額
    double newBalance = balance - amount;
    QString now = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    // 準備 UPDATE
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
    query.addBindValue(newBalance); // balance
    query.addBindValue(-amount);    // 最新交易金額
    query.addBindValue(newBalance); // 最新餘額
    query.addBindValue(now);        // 最新時間
    query.addBindValue(Controller::Getaccount());
    if (!query.exec()) {
        QMessageBox::critical(this, "資料庫錯誤", "提款失敗：" + query.lastError().text());
        ui->withdrow->clear();
        return;
    }
    Controller::Setbalance(newBalance);
    refreshBalance();
    emit showSummary("提款", amount, "", newBalance);
    ui->withdrow->clear();

}
void withdrowWindow::on_f1000_clicked() { withdraw(1000); }
void withdrowWindow::on_f2000_clicked() { withdraw(2000); }
void withdrowWindow::on_f3000_clicked() { withdraw(3000); }
void withdrowWindow::on_f5000_clicked() { withdraw(5000); }
void withdrowWindow::on_f10000_clicked() { withdraw(10000); }

void withdrowWindow::on_clear_clicked() { ui->withdrow->clear(); }

void withdrowWindow::on_withdraw_clicked() {
    bool ok = false;
    int amount = ui->withdrow->text().toInt(&ok);
    if (!ok || amount <= 0) {
        QMessageBox::warning(this, "錯誤", "請輸入正確的提款金額。");
        ui->withdrow->clear();
        return;
    }

    withdraw(amount);
}

void withdrowWindow::on_back_clicked() { emit back(); }
void withdrowWindow::on_logout_clicked() { emit logout(); }
