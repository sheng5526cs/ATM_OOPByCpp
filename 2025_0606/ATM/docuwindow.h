#ifndef DOCUWINDOW_H
#define DOCUWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include "controller.h"
#include "databasemanager.h"

namespace Ui {
class docuWindow;
}

class docuWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit docuWindow(QWidget *parent = nullptr);
    ~docuWindow();
    void setAccountAndBalance(const QString &account, double balance);
    void refreshTable(const QString &account);

signals:
    void back();
    void logout();

private slots:
    void on_back_clicked();
    void on_logout_clicked();

private:
    Ui::docuWindow *ui;
    DatabaseManager dbm;
};

#endif // DOCUWINDOW_H
