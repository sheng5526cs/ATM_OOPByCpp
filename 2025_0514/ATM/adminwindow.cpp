#include "adminwindow.h"
#include "ui_adminwindow.h"

adminwindow::adminwindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::adminwindow)
{
    ui->setupUi(this);
}

adminwindow::~adminwindow()
{
    delete ui;
}

void adminwindow::on_logout_clicked(){
    qDebug() << "click logout";
    emit logout();
}


void adminwindow::on_AccountlistButton_clicked(){
    qDebug() << "AccountlistButton clicked";
    emit Accountlist();
}

void adminwindow::on_AddAccountButton_clicked(){
    qDebug() << "AddAccountButton clicked";
    emit addaccountwindow();
}



void adminwindow::on_ResetpasswordButton_clicked(){
    qDebug() << "ResetpasswordButton clicked";
    emit passwordsetting();
}


