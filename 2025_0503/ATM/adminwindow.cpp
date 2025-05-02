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
