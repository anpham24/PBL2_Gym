#ifndef LOGINSYSTEM_H
#define LOGINSYSTEM_H

#include <string>
#include <vector>
#include <map>
#include "DataManager.h" // Để lấy định nghĩa UserRole

// Sử dụng namespace
using namespace std;

class LoginSystem {
private:
    map<string, User> users;
    string dataFile = "users.txt";

    // Hàm nội bộ để lưu/tải
    void saveUsersToFile();
    void loadUsersFromFile();

public:
    LoginSystem();

    bool login(const string& username, const string& password);
    User getUser(const string& username);
    bool isAdmin(const string& username);

    bool addStaffAccount(const string& adminUser, const string& newUsername, 
                        const string& newPassword);
    
    bool changePassword(const string& username, const string& oldPassword, 
                       const string& newPassword);
    
    vector<string> getAllStaffUsernames();
};

#endif // LOGINSYSTEM_H