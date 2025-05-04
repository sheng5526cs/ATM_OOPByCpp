#ifndef ACCLISTWINDOW_H
#define ACCLISTWINDOW_H

#include <QMainWindow>

namespace Ui {
class acclistWinow;
}

class acclistWinow : public QMainWindow
{
    Q_OBJECT

public:
    explicit acclistWinow(QWidget *parent = nullptr);
    ~acclistWinow();

    void loadAccounts();
private slots:
    void on_logout_clicked();
    void on_back_clicked();
private:
    Ui::acclistWinow *ui;
signals:
    void back();
    void logout();
};

#endif // ACCLISTWINDOW_H
