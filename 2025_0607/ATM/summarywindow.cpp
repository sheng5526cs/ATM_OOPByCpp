#include "summarywindow.h"
#include "ui_summarywindow.h"

summarywindow::summarywindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::summarywindow)
{
    ui->setupUi(this);
}

summarywindow::~summarywindow()
{
    delete ui;
}

void summarywindow::setSummary(const QString &type, int amount, const QString &target, double balance) {
    // 設定各標籤內容
    if (ui->label_type) ui->label_type->setText("交 易 類 型 ：" + type);
    if (ui->label_amount) ui->label_amount->setText("金 額 ：NT$ " + QString::number(amount));
    if (ui->label_target) {
        if (target.isEmpty()) {
            ui->label_target->setText("對 象 ： 無");
        } else {
            ui->label_target->setText("對 象 ： " + target);
        }
    }
    if (ui->label_balance) ui->label_balance->setText("結 餘 ：NT$ " + QString::number(balance, 'f', 0));
}

void summarywindow::on_back_clicked() {
    emit back();
}

void summarywindow::on_logout_clicked() {
    emit logout();
}
