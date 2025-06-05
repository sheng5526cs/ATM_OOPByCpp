#ifndef SETPASSWORDWINDOW_H
#define SETPASSWORDWINDOW_H

#include <QMainWindow>

namespace Ui {
class setpasswordwindow;
}

class setpasswordwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit setpasswordwindow(QWidget *parent = nullptr);
    ~setpasswordwindow();
private slots:
    void on_back_clicked();
    void on_logout_clicked();
    void on_change_clicked();
    void on_clear_clicked();
private:
    Ui::setpasswordwindow*ui;
signals:
    void back();
    void logout();
};

#endif // SETPASSWORDWINDOW_H
