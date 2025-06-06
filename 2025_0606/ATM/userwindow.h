#ifndef USERWINDOW_H
#define USERWINDOW_H
#include <QMainWindow>

namespace Ui {
class userwindow;
}

class userwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit userwindow(QWidget *parent = nullptr);
    ~userwindow();
    void setWelcomeText(const QString &name); // 設定主副標題

private slots:
    void on_logout_clicked();
    void on_changepass_clicked();
    void on_deposit_clicked();
    void on_withdrow_clicked();
    void on_trans_clicked();
    void on_docu_clicked();
    void on_idk_clicked();

private:
    Ui::userwindow *ui;

signals:
    void logout();
    void changepass();
    void deposit();
    void withdraw();
    void transfer();
    void transactionHistory();
};

#endif // USERWINDOW_H
