#include "transwindow.h"
#include "ui_transwindow.h"

TransWindow::TransWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TransWindow)
{
    ui->setupUi(this);

    refreshBalance();
}

TransWindow::~TransWindow()
{
    delete ui;
}

void TransWindow::refreshBalance() {
    double balance = Controller::Getbalance();
    ui->label_balance->setText(QString("目 前 餘 額 : %1 元").arg(balance, 0, 'f', 0));
}

void TransWindow::on_clear_clicked() {
    ui->account->clear();
    ui->amount->clear();
}

void TransWindow::on_trans_btn_clicked() {
    QString toAccount = ui->account->text().trimmed();
    bool ok = false;
    int amount = ui->amount->text().toInt(&ok);
    if (!ok || amount <= 0) {
        QMessageBox::warning(this, "錯誤", "請輸入正確的轉帳金額。");
        ui->amount->clear();
        ui->account->clear();
        return;
    }
    if (toAccount.isEmpty()) {
        QMessageBox::warning(this, "錯誤", "請輸入對方帳號。");
        ui->amount->clear();
        ui->account->clear();
        return;
    }
    if (toAccount == Controller::Getaccount()) {
        QMessageBox::warning(this, "錯誤", "不能轉帳給自己。");
        ui->amount->clear();
        ui->account->clear();
        return;
    }
    DatabaseManager dbm;
    if (!dbm.connect()) {
        QMessageBox::critical(this, "Database Error", "無法連線至系統。");
        ui->amount->clear();
        ui->account->clear();
        return;
    }
    QSqlDatabase db = QSqlDatabase::database("atm_connection");
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Database Error", "系統未開啟！");
        ui->amount->clear();
        ui->account->clear();
        return;
    }
    // 查詢自己餘額
    QSqlQuery balQuery(db);
    balQuery.prepare("SELECT balance FROM users WHERE account = ?");
    balQuery.addBindValue(Controller::Getaccount());
    if (!balQuery.exec() || !balQuery.next()) {
        QMessageBox::critical(this, "錯誤", "查詢餘額失敗");
        ui->amount->clear();
        ui->account->clear();
        return;
    }
    double myBalance = balQuery.value(0).toDouble();
    if (amount > myBalance) {
        QMessageBox::warning(this, "餘額不足", "您的餘額不足。");
        ui->amount->clear();
        ui->account->clear();
        return;
    }
    // 查詢對方帳號是否存在及餘額
    QSqlQuery toQuery(db);
    toQuery.prepare("SELECT balance FROM users WHERE account = ?");
    toQuery.addBindValue(toAccount);
    if (!toQuery.exec() || !toQuery.next()) {
        QMessageBox::warning(this, "錯誤", "對方帳號不存在。");
        ui->amount->clear();
        ui->account->clear();
        return;
    }
    double toBalance = toQuery.value(0).toDouble();
    // 取得自己舊的 1~4 筆交易紀錄
    QSqlQuery myOldQuery(db);
    myOldQuery.prepare("SELECT change_amount_1, balance_1, change_time_1, change_amount_2, balance_2, change_time_2, change_amount_3, balance_3, change_time_3, change_amount_4, balance_4, change_time_4 FROM users WHERE account = ?");
    myOldQuery.addBindValue(Controller::Getaccount());
    if (!myOldQuery.exec() || !myOldQuery.next()) {
        QMessageBox::critical(this, "錯誤", "查詢交易紀錄失敗");
        ui->amount->clear();
        ui->account->clear();
        return;
    }
    // 取得對方舊的 1~4 筆交易紀錄
    QSqlQuery toOldQuery(db);
    toOldQuery.prepare("SELECT change_amount_1, balance_1, change_time_1, change_amount_2, balance_2, change_time_2, change_amount_3, balance_3, change_time_3, change_amount_4, balance_4, change_time_4 FROM users WHERE account = ?");
    toOldQuery.addBindValue(toAccount);
    if (!toOldQuery.exec() || !toOldQuery.next()) {
        QMessageBox::critical(this, "錯誤", "查詢對方交易紀錄失敗");
        ui->amount->clear();
        ui->account->clear();
        return;
    }
    double myNewBalance = myBalance - amount;
    double toNewBalance = toBalance + amount;
    QString now = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    // 開始 transaction
    db.transaction();
    // 更新自己
    QSqlQuery myUpdate(db);
    myUpdate.prepare(R"(
        UPDATE users SET
            balance = ?,
            change_amount_5 = change_amount_4, balance_5 = balance_4, change_time_5 = change_time_4,
            change_amount_4 = change_amount_3, balance_4 = balance_3, change_time_4 = change_time_3,
            change_amount_3 = change_amount_2, balance_3 = balance_2, change_time_3 = change_time_2,
            change_amount_2 = change_amount_1, balance_2 = balance_1, change_time_2 = change_time_1,
            change_amount_1 = ?, balance_1 = ?, change_time_1 = ?
        WHERE account = ?
    )");
    myUpdate.addBindValue(myNewBalance);
    myUpdate.addBindValue(-amount);
    myUpdate.addBindValue(myNewBalance);
    myUpdate.addBindValue(now);
    myUpdate.addBindValue(Controller::Getaccount());
    if (!myUpdate.exec()) {
        db.rollback();
        QMessageBox::critical(this, "資料庫錯誤", "轉帳失敗(自己)：" + myUpdate.lastError().text());
        ui->amount->clear();
        ui->account->clear();
        return;
    }
    // 更新對方
    QSqlQuery toUpdate(db);
    toUpdate.prepare(R"(
        UPDATE users SET
            balance = ?,
            change_amount_5 = change_amount_4, balance_5 = balance_4, change_time_5 = change_time_4,
            change_amount_4 = change_amount_3, balance_4 = balance_3, change_time_4 = change_time_3,
            change_amount_3 = change_amount_2, balance_3 = balance_2, change_time_3 = change_time_2,
            change_amount_2 = change_amount_1, balance_2 = balance_1, change_time_2 = change_time_1,
            change_amount_1 = ?, balance_1 = ?, change_time_1 = ?
        WHERE account = ?
    )");
    toUpdate.addBindValue(toNewBalance);
    toUpdate.addBindValue(amount);
    toUpdate.addBindValue(toNewBalance);
    toUpdate.addBindValue(now);
    toUpdate.addBindValue(toAccount);
    if (!toUpdate.exec()) {
        db.rollback();
        QMessageBox::critical(this, "資料庫錯誤", "轉帳失敗(對方)：" + toUpdate.lastError().text());
        ui->amount->clear();
        ui->account->clear();
        return;
    }
    if (!db.commit()) {
        db.rollback();
        QMessageBox::critical(this, "資料庫錯誤", "轉帳失敗(提交失敗)：" + db.lastError().text());
        ui->amount->clear();
        ui->account->clear();
        return;
    }
    Controller::Setbalance(myNewBalance);
    refreshBalance();
    emit showSummary("轉帳", amount, toAccount, myNewBalance);
    ui->amount->clear();
    ui->account->clear();
}

void TransWindow::on_back_clicked() {
    emit back();
}

void TransWindow::on_logout_clicked() {
    emit logout();
}
