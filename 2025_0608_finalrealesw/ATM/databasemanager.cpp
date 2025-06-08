#include "DatabaseManager.h"
#include "controller.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCoreApplication>
#include <QFile>
#include <QCryptographicHash>
#include <QMessageBox>


DatabaseManager::DatabaseManager() {
    // 設定資料庫路徑為應用程式同一目錄
    QString dbPath = QCoreApplication::applicationDirPath() + "/atm.db";
    
    // 檢查資料庫檔案是否存在
    if (!QFile::exists(dbPath)) {
        qDebug() << "Database file not found at:" << dbPath;
        qDebug() << "Application dir path:" << QCoreApplication::applicationDirPath();
        
        // 嘗試尋找在專案目錄中的資料庫
        QString projectDbPath = QCoreApplication::applicationDirPath() + "/../../../atm.db";
        if (QFile::exists(projectDbPath)) {
            dbPath = projectDbPath;
            qDebug() << "Found database at project path:" << dbPath;
        }
    }
    
    qDebug() << "Using database path:" << dbPath;
}

bool DatabaseManager::connect() {
    QString connectionName = "atm_connection";

    // 若已存在連線名稱，就使用原本的連線
    if (QSqlDatabase::contains(connectionName)) {
        db = QSqlDatabase::database(connectionName);
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        
        // 設定資料庫路徑為應用程式同一目錄
        QString dbPath = QCoreApplication::applicationDirPath() + "/atm.db";
        
        // 檢查資料庫檔案是否存在
        if (!QFile::exists(dbPath)) {
            qDebug() << "Database file not found at:" << dbPath;
            qDebug() << "Application dir path:" << QCoreApplication::applicationDirPath();
            
            // 嘗試尋找在專案目錄中的資料庫
            QString projectDbPath = QCoreApplication::applicationDirPath() + "/../../../atm.db";
            if (QFile::exists(projectDbPath)) {
                dbPath = projectDbPath;
                qDebug() << "Found database at project path:" << dbPath;
            } else {
                qDebug() << "Database file not found in any location!";
                QMessageBox::critical(nullptr, "資料庫錯誤", 
                    "找不到資料庫檔案 atm.db\n請確保資料庫檔案與程式在同一目錄");
                return false;
            }
        }
        
        qDebug() << "Using database path:" << dbPath;
        db.setDatabaseName(dbPath);
    }

    if (!db.isOpen()) {
        if (db.open()) {
            qDebug() << "Database opened successfully.";
            return true;
        } else {
            qDebug() << "Failed to open the database:" << db.lastError().text();
            QMessageBox::critical(nullptr, "資料庫連線錯誤", 
                QString("無法開啟資料庫：%1").arg(db.lastError().text()));
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
        Controller::Setid(query.value(0).toString());     // id
        Controller::Setbalance(query.value(1).toDouble()); // balance
        return true;
    }

    return false;  // 沒找到資料
}


int DatabaseManager::fetchPassword(const QString &account, const QString &inputPassword) {
    QSqlQuery query(db);
    query.prepare("SELECT password, status FROM users WHERE account = ?");
    query.addBindValue(account);

    if (!query.exec()) {
        qDebug() << "Failed to fetch user data:" << query.lastError().text();
        return -1; // 查詢失敗
    }

    if (query.next()) {
        QString storedHashPassword = query.value(0).toString();
        int status = query.value(1).toInt();

        if (status == 3) {
            return 9; // 鎖定狀態
        }

        QByteArray hashedInput = QCryptographicHash::hash(inputPassword.toUtf8(), QCryptographicHash::Sha256).toHex();
        QSqlQuery updateQuery(db);
        updateQuery.prepare("UPDATE users SET status = ? WHERE account = ?");

        if(status == 500) {
            if(storedHashPassword == hashedInput)return 501;
            return 500;// 管理員
        }


        if (storedHashPassword == hashedInput) {
            status=0;
            updateQuery.addBindValue(status);
            updateQuery.addBindValue(account);

            if (!updateQuery.exec()) {
                qDebug() << "[DEBUG] Failed to reset status:" << updateQuery.lastError().text();
            }
            return 1; // 密碼正確
        } else if (status < 3) {
            status++;
            updateQuery.addBindValue(status);
            updateQuery.addBindValue(account);

            if (!updateQuery.exec()) {
                qDebug() << "[DEBUG] Update error:" << updateQuery.lastError().text();
            }

            return 0; // 密碼錯誤
        }
    }

    return -2; // 找不到帳號
}

void DatabaseManager::status(const QString &account,int index){

}
