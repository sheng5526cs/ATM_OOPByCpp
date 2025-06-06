#ifndef TRANSWINDOW_H
#define TRANSWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include "controller.h"
#include "databasemanager.h"

namespace Ui {
class TransWindow;
}

class TransWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TransWindow(QWidget *parent = nullptr);
    ~TransWindow();
    void refreshBalance();

signals:
    void back();
    void logout();

public slots:
    void on_trans_btn_clicked();
    void on_clear_clicked();
    void on_back_clicked();
    void on_logout_clicked();

private:
    Ui::TransWindow *ui;

};

#endif // TRANSWINDOW_H
