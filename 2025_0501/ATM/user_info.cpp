// user_info.cpp
#include "user_info.h"

UserInfo::UserInfo() : balance(0.0) {}

void UserInfo::setAccount(const QString& account) {
    this->account = account;
}

void UserInfo::setPassword(const QString& password) {
    this->password = password;
}

void UserInfo::setBalance(double balance) {
    this->balance = balance;
}

QString UserInfo::getAccount() const {
    return account;
}

QString UserInfo::getPassword() const {
    return password;
}

double UserInfo::getBalance() const {
    return balance;
}
