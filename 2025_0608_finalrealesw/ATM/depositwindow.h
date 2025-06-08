#ifndef DEPOSITWINDOW_H
#define DEPOSITWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include "controller.h"
#include "databasemanager.h"

namespace Ui {
class depositWindow;
}

class depositWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit depositWindow(QWidget *parent = nullptr);
    ~depositWindow();
    void refreshBalance();

signals:
    void back();
    void logout();
    void showSummary(const QString &type, int amount, const QString &target, double balance); // 新增交易摘要signal

private slots:
    void on_clear_clicked();
    void on_deposit_btn_clicked();
    void on_back_clicked();
    void on_logout_clicked();

private:
    Ui::depositWindow *ui;
    DatabaseManager dbm;
};

#endif // DEPOSITWINDOW_H
