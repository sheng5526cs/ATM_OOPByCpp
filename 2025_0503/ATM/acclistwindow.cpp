#include "acclistwindow.h"
#include "ui_acclistwindow.h"

acclistWinow::acclistWinow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::acclistWinow)
{
    ui->setupUi(this);
}

acclistWinow::~acclistWinow()
{
    delete ui;
}
