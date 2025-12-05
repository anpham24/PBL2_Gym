#include "AccountManager.h"
#include <iostream>

AccountManager::AccountManager()
{
    // Khong tu dong tao admin o day nua.
    // Viec nay se do App.cpp xu ly sau khi load file.
    // std::cout << "Dang khoi tao AccountManager..." << std::endl;
}

AccountManager::~AccountManager()
{
    // Giai phong bo nho cho tat ca cac doi tuong Account da cap phat
    for (size_t i = 0; i < dsAccount.size(); ++i)
    {
        if (dsAccount[i] != nullptr)
        {
            delete dsAccount[i];
        }
    }
    dsAccount.clear();
    // std::cout << "Da huy AccountManager." << std::endl;
}

Account *AccountManager::findAccount(const std::string &username)
{
    for (size_t i = 0; i < dsAccount.size(); ++i)
    {
        if (dsAccount[i] != nullptr && dsAccount[i]->getUsername() == username)
        {
            return dsAccount[i];
        }
    }
    return nullptr;
}

Account *AccountManager::validate(const std::string &username, const std::string &password)
{
    Account *acc = findAccount(username);
    if (acc != nullptr)
    {
        if (acc->checkPassword(password))
        {
            return acc;
        }
    }
    return nullptr;
}

Account *AccountManager::getAccount(const std::string &username)
{
    return findAccount(username);
}

bool AccountManager::addAccount(const std::string &username, const std::string &password, AccountType type, NhanVien *staff)
{
    if (findAccount(username) != nullptr)
    {
        std::cerr << "Loi (AccountManager): Ten dang nhap '" << username << "' da ton tai!" << std::endl;
        return false;
    }

    Account *newAcc = new Account(username, password, type, staff);
    dsAccount.push_back(newAcc);
    std::cout << "Da them tai khoan moi: " << username << std::endl;
    return true;
}

bool AccountManager::removeAccount(const std::string &username)
{
    if (username == "admin")
    {
        std::cerr << "Loi: Khong the xoa tai khoan admin!" << std::endl;
        return false;
    }

    for (size_t i = 0; i < dsAccount.size(); ++i)
    {
        if (dsAccount[i] != nullptr && dsAccount[i]->getUsername() == username)
        {
            delete dsAccount[i];
            dsAccount.erase(i);
            std::cout << "Da xoa tai khoan: " << username << std::endl;
            return true;
        }
    }
    return false;
}

bool AccountManager::changePassword(const std::string &username, const std::string &newPassword)
{
    Account *acc = findAccount(username);
    if (acc != nullptr)
    {
        acc->setPassword(newPassword);
        std::cout << "Da doi mat khau cho: " << username << std::endl;
        return true;
    }
    return false;
}

// Ham moi de FileIO su dung
const MyVector<Account *> &AccountManager::getDsAccount() const
{
    return dsAccount;
}