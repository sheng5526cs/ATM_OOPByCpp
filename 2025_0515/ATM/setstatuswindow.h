#ifndef SETSTATUSWINDOW_H
#define SETSTATUSWINDOW_H

#include <QMainWindow>

namespace Ui {
class setstatuswindow;
}

class setstatuswindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit setstatuswindow(QWidget *parent = nullptr);
    ~setstatuswindow();
private slots:
    void on_back_clicked();
    void on_logout_clicked();
    void on_change_clicked();
    void on_clear_clicked();
private:
    Ui::setstatuswindow *ui;
signals:
    void back();
    void logout();
};

#endif // SETSTATUSWINDOW_H
