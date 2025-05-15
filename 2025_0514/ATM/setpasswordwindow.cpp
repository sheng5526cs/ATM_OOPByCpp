#include "setpasswordwindow.h"
#include "ui_setpasswordwindow.h"

setpasswordwindow::setpasswordwindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::setpasswordwindow)
{
    ui->setupUi(this);
}

setpasswordwindow::~setpasswordwindow()
{
    delete ui;
}



void setpasswordwindow::on_back_clicked(){
    qDebug()<<"on_back_clicked()";
    emit back();
}
void setpasswordwindow::on_logout_clicked(){
    qDebug()<<"on_logout_clicked()";
    emit logout();
}


void setpasswordwindow::on_change_clicked(){
    qDebug()<<"on_change_clicked()";
}
void setpasswordwindow::on_clear_clicked(){
    qDebug()<<"on_clear_clicked()";

}
