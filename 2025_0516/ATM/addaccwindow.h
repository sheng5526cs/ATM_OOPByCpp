#ifndef ADDACCWINDOW_H
#define ADDACCWINDOW_H

#include <QMainWindow>

namespace Ui {
class addaccwindow;
}

class addaccwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit addaccwindow(QWidget *parent = nullptr);
    ~addaccwindow();
private slots:
    void on_back_clicked();
    void on_logout_clicked();
    void on_clear_clicked();
    void on_add_clicked();
private:
    Ui::addaccwindow *ui;
signals:
    void back();
    void logout();
};

#endif // ADDACCWINDOW_H
