#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>

class DatabaseManager {
public:
    DatabaseManager();
    bool connect();               // 連線資料庫

    void createUSERTable();
    void insertTestUserData();
    bool fetchUserData(const QString &account);

    int fetchPassword(const QString &account,const QString &password);

    void status(const QString &account,int index);
private:
    QSqlDatabase db;
};

#endif // DATABASEMANAGER_H
