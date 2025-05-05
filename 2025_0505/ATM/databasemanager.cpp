#include "DatabaseManager.h"
#include "controller.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCoreApplication>
#include <QFile>
#include <QCryptographicHash>


DatabaseManager::DatabaseManager() {
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


void DatabaseManager::createUSERTable() { //創造users用 別刪 別動
        // 檢查資料庫連線是否開啟
        if (!db.isOpen()) {
            qDebug() << "Database is not connected. Please connect first.";

        }

        QSqlQuery query(db);
        QString sql = R"(
        CREATE TABLE users (
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
            change_time_5 TEXT,
            balance REAL DEFAULT 0
        );

    )";

        if (!query.exec(sql)) {
            qDebug() << "Failed to create users table:" << query.lastError().text();

        }

        qDebug() << "Users table created successfully!";

    }

void DatabaseManager::insertTestUserData() {
    if (!db.isOpen()) {
        qDebug() << "Database is not connected. Please connect first.";
        return;
    }

    // 檢查 users 資料表是否有資料
    QSqlQuery checkQuery(db);
    if (!checkQuery.exec("SELECT COUNT(*) FROM users")) {
        qDebug() << "Failed to check user count:" << checkQuery.lastError().text();
        return;
    }

    checkQuery.next();
    int userCount = checkQuery.value(0).toInt();

    if (userCount > 0) {
        qDebug() << "User table already has data. Skipping test data insertion.";
        return;
    }

    // 插入測試資料
    QSqlQuery insertQuery(db);
    insertQuery.prepare(R"(
        INSERT INTO users (account, password, status, change_amount_1, balance_1, change_time_1)
        VALUES (?, ?, ?, ?, ?, ?)
    )");

    QString plainPassword = "55688";
    QByteArray hashed = QCryptographicHash::hash(plainPassword.toUtf8(), QCryptographicHash::Sha256);
    QString hashedPassword = hashed.toHex();

    insertQuery.addBindValue("sqltester");    // account
    insertQuery.addBindValue(hashedPassword); // hashed password
    insertQuery.addBindValue(0);              // status
    insertQuery.addBindValue(0);              // change_amount_1
    insertQuery.addBindValue(0);              // balance_1
    insertQuery.addBindValue("");             // change_time_1

    if (!insertQuery.exec()) {
        qDebug() << "Failed to insert test user data:" << insertQuery.lastError().text();
    } else {
        qDebug() << "Test user data inserted successfully. Hashed password:" << hashedPassword;
    }
}




bool DatabaseManager::fetchUserData(const QString &account) {

    QSqlQuery query(db);
    query.prepare("SELECT id, balance FROM users WHERE account = ?");
    query.addBindValue(account);

    if (!query.exec()) {
        qDebug() << "Failed to fetch user data:" << query.lastError().text();
        return false;  // 查詢失敗
    }

    if (query.next()) {
        // 從資料庫獲取資料
        Controller::Setaccount(account);
        Controller::Setid(query.value(0).toString());     // 假設 ID 在第一欄
        Controller::Setbalance(query.value(19).toDouble()); // 假設餘額在第二欄
        return true;
    }

    return false;  // 沒找到資料
}


bool DatabaseManager::fetchPassword(const QString &account, const QString &inputPassword) {
    // 準備查詢
    QSqlQuery query(db);
    query.prepare("SELECT password FROM users WHERE account = ?");
    query.addBindValue(account);

    if (!query.exec()) {
        qDebug() << "Failed to fetch user data:" << query.lastError().text();
        return false; // 查詢失敗
    }

    if (query.next()) {
        // 從資料庫取得雜湊密碼
        QString storedHashPassword = query.value(0).toString();

        // 將輸入的密碼進行 SHA-256 雜湊
        QByteArray hashedInput = QCryptographicHash::hash(inputPassword.toUtf8(), QCryptographicHash::Sha256).toHex();

        // 比對雜湊值
        if (storedHashPassword == hashedInput) {
            return true;  // 密碼正確
        }
    }

    return false;  // 密碼錯誤或找不到帳號
}


