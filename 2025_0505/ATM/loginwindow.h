#ifndef LOGINWINDOW_H // 防止頭文件重複包含
#define LOGINWINDOW_H


#include <QMainWindow> // 引入 QMainWindow 基類，提供主視窗功能

QT_BEGIN_NAMESPACE
namespace Ui { class loginWindow; } // 宣告 UI 命名空間，引用 loginwindow.ui 生成的類
QT_END_NAMESPACE

class loginWindow : public QMainWindow { // 定義 loginWindow 類，繼承 QMainWindow
    Q_OBJECT // 啟用 Qt 的信號槽機制

public:
    loginWindow(QWidget *parent = nullptr); // 建構函數，接受父物件指標，預設為空
    ~loginWindow(); // 解構函數，清理資源
    void clearPassword();  // 宣告清除


private slots:
    void on_LoginButton_clicked(); // 私有槽函數，處理 LoginButton 的 clicked 信號

private:
    Ui::loginWindow *ui; // 指向 UI 物件的指標，用於訪問 loginwindow.ui 的元件

signals:
    void loginPress(const QString &account, const QString &password ,const bool &admin);  // 新增這個版本


};

#endif // LOGINWINDOW_H // 結束頭文件保護
