#include "changepasswordwindow.h"
#include "ui_changepasswordwindow.h"

changepasswordWindow::changepasswordWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::changepasswordWindow)
{
    ui->setupUi(this);
}

changepasswordWindow::~changepasswordWindow()
{
    delete ui;
}
