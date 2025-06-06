#include "acclistwindow.h"
#include "databasemanager.h"
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlError>
#include <QDebug>
#include "ui_acclistwindow.h"

acclistWindow::acclistWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::acclistWinow)
{
    ui->setupUi(this);
    loadAccounts();
}

acclistWindow::~acclistWindow()
{
    delete ui;
}
void acclistWindow::loadAccounts() {
    ui->accountlist->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->accountlist->verticalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    DatabaseManager dbm;
    if (!dbm.connect()) {
        qDebug() << "Database Error";
        return;
    }

    QSqlDatabase db = QSqlDatabase::database("atm_connection");

    if (!db.isOpen()) {
        qDebug() << "[DEBUG] Database is NOT open in acclistWinow:" << db.lastError().text();
        return;
    }

    // 創建模型並設定資料庫
    model = new QSqlTableModel(this, db);
    model->setTable("users");

    model->setEditStrategy(QSqlTableModel::OnRowChange);
    // 嘗試選擇資料
    if (!model->select()) {
        qDebug() << "[DEBUG] Select failed:" << model->lastError().text();
        return;
    }

    // 設定欄位名稱
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Account");
    model->setHeaderData(3, Qt::Horizontal, "Status");
    model->setHeaderData(19, Qt::Horizontal, "Balance");

    // 設定表格模型
    ui->accountlist->setModel(model);

    ui->accountlist->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // 隱藏第2列，第4到第18列
    ui->accountlist->setColumnHidden(2, true);  // 隱藏第2列
    for (int i = 4; i <= 18; ++i) {
        ui->accountlist->setColumnHidden(i, true);  // 隱藏第4到第18列
    }

    // 讓表格欄位自動調整大小
    ui->accountlist->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}


void acclistWindow::on_logout_clicked(){
    qDebug()<<"acclistWinow::on_logout_clicked";
    emit logout();
}
void acclistWindow::on_back_clicked(){
    qDebug()<<"acclistWinow::on_back_clicked";
    emit back();
}


void acclistWindow::refresh(){
    if (!model->select()) {
        qDebug() << "[DEBUG] Select failed:" << model->lastError().text();
    } else {
        qDebug() << "[DEBUG] Model refreshed";
    }
}
