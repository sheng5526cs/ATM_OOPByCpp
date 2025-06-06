#ifndef ACCLISTWINDOW_H
#define ACCLISTWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
namespace Ui {
class acclistWinow;
}

class acclistWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit acclistWindow(QWidget *parent = nullptr);
    ~acclistWindow();

    void loadAccounts();
    void refresh();
private slots:
    void on_logout_clicked();
    void on_back_clicked();
private:
    Ui::acclistWinow *ui;
    QSqlTableModel *model;
signals:
    void back();
    void logout();
};

#endif // ACCLISTWINDOW_H
