#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCoreApplication>
#include <QFile>

DatabaseManager::DatabaseManager() {
    // 移除這一行：避免重複 addDatabase
    // db = QSqlDatabase::addDatabase("QSQLITE");
    QString dbPath = QCoreApplication::applicationDirPath() + "/../../../atm.db";
    db.setDatabaseName(dbPath);
}

bool DatabaseManager::connect() {
    QString connectionName = "atm_connection";

    // 動過：若已存在連線名稱，就使用原本的連線
    if (QSqlDatabase::contains(connectionName)) {
        db = QSqlDatabase::database(connectionName);
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        QString dbPath = QCoreApplication::applicationDirPath() + "/../../../atm.db";
        db.setDatabaseName(dbPath);
    }

    if (!db.isOpen()) {
        if (db.open()) {
            qDebug() << "Database opened successfully.";
            return true;
        } else {
            qDebug() << "Failed to open the database:" << db.lastError().text();
            return false;
        }
    }

    qDebug() << "Database was already open.";
    return true;
}


void DatabaseManager::createUSERTable() { //創造user用 別刪
        // 檢查資料庫連線是否開啟
        if (!db.isOpen()) {
            qDebug() << "Database is not connected. Please connect first.";

        }

        QSqlQuery query(db);
        QString sql = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            account TEXT NOT NULL UNIQUE,
            password TEXT NOT NULL,
            status INTEGER,
            change_amount_1 REAL,
            balance_1 REAL,
            change_time_1 TEXT,
            change_amount_2 REAL,
            balance_2 REAL,
            change_time_2 TEXT,
            change_amount_3 REAL,
            balance_3 REAL,
            change_time_3 TEXT,
            change_amount_4 REAL,
            balance_4 REAL,
            change_time_4 TEXT,
            change_amount_5 REAL,
            balance_5 REAL,
            change_time_5 TEXT
        )
    )";

        if (!query.exec(sql)) {
            qDebug() << "Failed to create users table:" << query.lastError().text();

        }

        qDebug() << "Users table created successfully!";

    }


void DatabaseManager::insertTestUserData() { //測資用
    // 檢查資料庫連線是否開啟
    if (!db.isOpen()) {
        qDebug() << "Database is not connected. Please connect first.";
        return;
    }

    // 準備插入測試資料
    QSqlQuery query(db);
    query.prepare(R"(
    INSERT INTO users (account, password, status, change_amount_1, balance_1, change_time_1)
    VALUES (?, ?, ?, ?, ?, ?)
)");

    // 綁定資料
    query.addBindValue("sqltester"); // account
    query.addBindValue("55688");     // password
    query.addBindValue(0);           // status
    query.addBindValue(0);           // change_amount_1
    query.addBindValue(0);           // balance_1
    query.addBindValue("");          // change_time_1 (empty string for now)

    if (!query.exec()) {
        qDebug() << "Failed to insert test user data:" << query.lastError().text();
    } else {
        qDebug() << "Test user data inserted successfully.";
    }
}


void DatabaseManager::fetchUserData() { //查資料
    // 檢查資料庫連線是否開啟
    if (!db.isOpen()) {
        qDebug() << "Database is not connected. Please connect first.";
        return;
    }

    // 準備查詢資料
    QSqlQuery query(db);
    query.prepare("SELECT * FROM users WHERE account = ?");

    // 綁定帳號條件
    query.addBindValue("sqltester");

    if (!query.exec()) {
        qDebug() << "Failed to fetch user data:" << query.lastError().text();
        return;
    }

    // 顯示查詢結果
    while (query.next()) {
        int id = query.value(0).toInt();
        QString account = query.value(1).toString();
        QString password = query.value(2).toString();
        int status = query.value(3).toInt();
        double changeAmount1 = query.value(4).toDouble();
        double balance1 = query.value(5).toDouble();
        QString changeTime1 = query.value(6).toString();

        qDebug() << "User Data:";
        qDebug() << "ID:" << id;
        qDebug() << "Account:" << account;
        qDebug() << "Password:" << password;
        qDebug() << "Status:" << status;
        qDebug() << "Change Amount 1:" << changeAmount1;
        qDebug() << "Balance 1:" << balance1;
        qDebug() << "Change Time 1:" << changeTime1;
    }
}


QString DatabaseManager::fetchPassword(const QString &account) {
    QSqlQuery query(db);
    query.prepare("SELECT password FROM users WHERE account = ?");
    query.addBindValue(account);

    if (!query.exec()) {
        qDebug() << "Failed to fetch user data:" << query.lastError().text();
        return "Failed";
    }

    if (query.next()) {
        return query.value(0).toString();  // password
    } else {
        return "NOT_FOUND";  // 沒找到帳號
    }
}
