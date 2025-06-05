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

private:
    Ui::changepasswordWindow *ui;
};

#endif // CHANGEPASSWORDWINDOW_H
