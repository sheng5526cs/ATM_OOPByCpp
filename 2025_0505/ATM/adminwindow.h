#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>

namespace Ui {
class adminwindow;
}

class adminwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit adminwindow(QWidget *parent = nullptr);
    ~adminwindow();


private slots:
    void on_logout_clicked();
    void on_AccountlistButton_clicked();
    void on_AddAccountButton_clicked();
private:
    Ui::adminwindow *ui;

signals:
    void logout();
    void Accountlist();
    void addaccountwindow();
};

#endif // ADMINWINDOW_H
