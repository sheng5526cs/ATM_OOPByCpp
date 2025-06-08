#ifndef SUMMARYWINDOW_H
#define SUMMARYWINDOW_H

#include <QMainWindow>

namespace Ui {
class summarywindow;
}

class summarywindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit summarywindow(QWidget *parent = nullptr);
    ~summarywindow();
    void setSummary(const QString &type, int amount, const QString &target, double balance); // 設定摘要內容

signals:
    void back();    // 繼續操作（返回主選單）
    void logout();  // 登出

private slots:
    void on_back_clicked();    // 繼續操作按鈕處理
    void on_logout_clicked();  // 登出按鈕處理

private:
    Ui::summarywindow *ui;
};

#endif // SUMMARYWINDOW_H
