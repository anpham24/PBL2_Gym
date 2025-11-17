// Account.cpp
#include "Account.h"

Account::Account(const std::string& user, const std::string& pass, AccountType t, NhanVien* staff)
    : username(user), type(t), linkedStaff(staff) {
    
    // Luu mat khau truc tiep (plaintext)
    this->hashedPassword = pass; 
}

Account::~Account() {}

const std::string& Account::getUsername() const { return username; }
AccountType Account::getAccountType() const { return type; }
NhanVien* Account::getLinkedStaff() const { return linkedStaff; }

const std::string& Account::getPassword() const {
    return hashedPassword;
}
// Kiem tra mat khau don gian
bool Account::checkPassword(const std::string& pass) const {
    return pass == hashedPassword;
}

// Dat mat khau don gian
void Account::setPassword(const std::string& newPass) {
    hashedPassword = newPass;
}