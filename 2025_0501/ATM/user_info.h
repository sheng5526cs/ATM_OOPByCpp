// user_info.h
#ifndef USER_INFO_H
#define USER_INFO_H

#include <QString>

class UserInfo {
public:
    UserInfo();
    void setAccount(const QString& account);
    void setPassword(const QString& password);
    void setBalance(double balance);

    QString getAccount() const;
    QString getPassword() const;
    double getBalance() const;

private:
    QString account;
    QString password;
    double balance;
};

#endif // USER_INFO_H
