#include "loginwindow.h"
#include "ui_loginwindow.h"

loginWindow::loginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::loginWindow)
{
    ui->setupUi(this);
}

loginWindow::~loginWindow()
{
    delete ui;
}
