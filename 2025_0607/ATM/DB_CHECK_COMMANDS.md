# 數據庫驗證命令

## 查看帳戶餘額
sqlite3 atm.db "SELECT account, balance FROM users WHERE account IN ('123', 'sqltester', 'sheng');"

## 查看交易記錄 (最新5筆)
sqlite3 atm.db "SELECT account, change_amount_1, balance_1, change_time_1 FROM users WHERE account='123';"

## 查看所有帳戶狀態
sqlite3 atm.db "SELECT id, account, status, balance FROM users;"

## 檢查新增的測試帳戶
sqlite3 atm.db "SELECT * FROM users WHERE account='testuser';"
