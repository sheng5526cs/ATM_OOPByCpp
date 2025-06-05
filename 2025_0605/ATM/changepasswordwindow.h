#ifndef CHANGEPASSWORDWINDOW_H
#define CHANGEPASSWORDWINDOW_H

#include <QMainWindow>

namespace Ui {
class changepasswordWindow;
}

class changepasswordWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit changepasswordWindow(QWidget *parent = nullptr);
    ~changepasswordWindow();

signals:
    void back();
    void logout();

private slots:
    void on_back_clicked();
    void on_logout_clicked();
    void on_clear_clicked();
    void on_check_clicked();

private:
    Ui::changepasswordWindow *ui;
};

#endif // CHANGEPASSWORDWINDOW_H


