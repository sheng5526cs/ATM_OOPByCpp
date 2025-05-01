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


private slots:
    void on_logout_clicked();
private:
    Ui::userwindow *ui;
signals:
    void logout();

};

#endif // USERWINDOW_H
