#pragma once

#include <string>
#include "Utils/MyVector.h" 
#include "Models/Account.h"  
#include "Models/NhanVien.h" 

class AccountManager {
private:
    MyVector<Account*> dsAccount; 
    Account* findAccount(const std::string& username);

public:
    AccountManager();
    ~AccountManager();
    Account* validate(const std::string& username, const std::string& password);
    Account* getAccount(const std::string& username); 
    bool addAccount(const std::string& username, const std::string& password, AccountType type, NhanVien* staff = nullptr);
    bool removeAccount(const std::string& username);
    bool changePassword(const std::string& username, const std::string& newPassword);
    const MyVector<Account*>& getDsAccount() const;
};