#ifndef WITHDROWWINDOW_H
#define WITHDROWWINDOW_H

#include <QMainWindow>
#include "controller.h"
#include "databasemanager.h"
#include <QMessageBox>

namespace Ui {
class withdrowWindow;
}

class withdrowWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit withdrowWindow(QWidget *parent = nullptr);
    ~withdrowWindow();
    void refreshBalance();

signals:
    void back();
    void logout();

private slots:
    void on_f1000_clicked();
    void on_f2000_clicked();
    void on_f3000_clicked();
    void on_f5000_clicked();
    void on_f10000_clicked();
    void on_clear_clicked();
    void on_withdraw_clicked();
    void on_back_clicked();
    void on_logout_clicked();


private:
    Ui::withdrowWindow *ui;
    DatabaseManager dbm;

};

#endif // WITHDROWWINDOW_H
