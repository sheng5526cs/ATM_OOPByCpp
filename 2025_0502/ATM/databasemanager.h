#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>

class DatabaseManager {
public:
    DatabaseManager();
    bool connect();               // 連線資料庫

    void createUSERTable();
    void insertTestUserData();
    void fetchUserData();
    QString fetchPassword(const QString &account);

private:
    QSqlDatabase db;
};

#endif // DATABASEMANAGER_H
