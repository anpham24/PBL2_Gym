// Account.h
#pragma once
#include <string>

#include "NhanVien.h" 

enum class AccountType {
    ADMIN,
    STAFF
};

class Account {
private:
    std::string username;
    std::string hashedPassword; // Nen luu hash, khong luu plaintext
    AccountType type;
    NhanVien* linkedStaff; // Con tro den NhanVien tuong ung, co a nullptr neu la Admin

public:
    Account(const std::string& user, const std::string& pass, AccountType t, NhanVien* staff = nullptr);
    ~Account();

    const std::string& getUsername() const;
    AccountType getAccountType() const;
    NhanVien* getLinkedStaff() const;
    
    const std::string& getPassword() const;
    bool checkPassword(const std::string& pass) const;
    void setPassword(const std::string& newPass); // Nen hash truoc khi set
};