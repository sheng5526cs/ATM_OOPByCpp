#include "docuwindow.h"
#include "ui_docuwindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QDateTime>
#include <QHeaderView>

docuWindow::docuWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::docuWindow)
{
    ui->setupUi(this);
    connect(ui->back, &QPushButton::clicked, this, &docuWindow::on_back_clicked);
    connect(ui->logout, &QPushButton::clicked, this, &docuWindow::on_logout_clicked);
}

docuWindow::~docuWindow()
{
    delete ui;
}

void docuWindow::setAccountAndBalance(const QString &account, double balance) {
    ui->label_account->setText(QString("帳 號 ：%1").arg(account));
    ui->label_balance->setText(QString("目 前 餘 額 : %1 元").arg(balance, 0, 'f', 0));
}

void docuWindow::refreshTable(const QString &account) {
    // 取得最新 5 筆交易紀錄
    if (!dbm.connect()) {
        QMessageBox::critical(this, "資料庫錯誤", "無法連線至系統。");
        return;
    }
    QSqlDatabase db = QSqlDatabase::database("atm_connection");
    QSqlQuery query(db);
    query.prepare(R"(
        SELECT change_time_1, change_amount_1, balance_1,
               change_time_2, change_amount_2, balance_2,
               change_time_3, change_amount_3, balance_3,
               change_time_4, change_amount_4, balance_4,
               change_time_5, change_amount_5, balance_5
        FROM users WHERE account = ?
    )");
    query.addBindValue(account);
    if (!query.exec() || !query.next()) {
        QMessageBox::critical(this, "查詢失敗", "查詢交易紀錄失敗");
        return;
    }
    ui->tableWidget->setRowCount(0);
    int col = 0;
    for (int i = 0; i < 5; ++i) {
        QString time = query.value(col++).toString();
        double amount = query.value(col++).toDouble();
        double bal = query.value(col++).toDouble();
        if (time.isEmpty()) continue;
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        QTableWidgetItem *item0 = new QTableWidgetItem(time);
        QTableWidgetItem *item1 = new QTableWidgetItem(QString::number(amount, 'f', 0));
        QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(bal, 'f', 0));
        item0->setTextAlignment(Qt::AlignCenter);
        item1->setTextAlignment(Qt::AlignCenter);
        item2->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(row, 0, item0);
        ui->tableWidget->setItem(row, 1, item1);
        ui->tableWidget->setItem(row, 2, item2);
    }
    // 欄寬自動縮放，時間50%，金額/餘額各25%
    QHeaderView *header = ui->tableWidget->horizontalHeader();
    header->setSectionResizeMode(0, QHeaderView::Stretch);
    header->setSectionResizeMode(1, QHeaderView::Stretch);
    header->setSectionResizeMode(2, QHeaderView::Stretch);
    // verticalHeader 也自動填滿
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // 表頭置中
    ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    // 欄位比例
    int total = 4;
    ui->tableWidget->setColumnWidth(0, ui->tableWidget->width() * 2 / total);
    ui->tableWidget->setColumnWidth(1, ui->tableWidget->width() * 1 / total);
    ui->tableWidget->setColumnWidth(2, ui->tableWidget->width() * 1 / total);
}

void docuWindow::on_back_clicked() { emit back(); }
void docuWindow::on_logout_clicked() { emit logout(); }
