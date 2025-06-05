#include "userwindow.h"
#include "ui_userwindow.h"

userwindow::userwindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::userwindow)
{
    ui->setupUi(this);
}

userwindow::~userwindow()
{
    delete ui;
}


void userwindow::on_logout_clicked(){
    qDebug() << "click logout";
    emit logout();
}

void userwindow::on_changepass_clicked(){
    qDebug() << "click changepass";
    emit changepass();
}
