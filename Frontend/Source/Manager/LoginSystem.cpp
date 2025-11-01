#include "LoginSystem.h"
#include <fstream>
#include <sstream>

// Sử dụng namespace
using namespace std;

LoginSystem::LoginSystem() {
    loadUsersFromFile();
    
    // Nếu không có user nào, tạo tài khoản mặc định
    if (users.empty()) {
        users["admin"] = {"admin", "admin123", UserRole::ADMIN};
        users["staff"] = {"staff", "staff123", UserRole::STAFF};
        saveUsersToFile();
    }
}

bool LoginSystem::login(const string& username, const string& password) {
    if (users.find(username) != users.end() && 
        users[username].password == password) {
        return true;
    }
    return false;
}

User LoginSystem::getUser(const string& username) {
    // Trả về user, nếu không tìm thấy thì trả về một user rỗng
    if (users.find(username) != users.end()) {
        return users[username];
    }
    return User(); 
}

bool LoginSystem::isAdmin(const string& username) {
    if (users.find(username) != users.end()) {
        return users[username].role == UserRole::ADMIN;
    }
    return false;
}

bool LoginSystem::addStaffAccount(const string& adminUser, const string& newUsername, 
                                  const string& newPassword) {
    // Chỉ admin mới có quyền thêm staff
    if (users.find(adminUser) == users.end() || users[adminUser].role != UserRole::ADMIN) {
        return false;
    }
    
    // Kiểm tra xem user mới đã tồn tại chưa
    if (users.find(newUsername) != users.end()) {
        return false;
    }

    users[newUsername] = {newUsername, newPassword, UserRole::STAFF};
    saveUsersToFile();
    return true;
}

bool LoginSystem::changePassword(const string& username, const string& oldPassword, 
                                 const string& newPassword) {
    if (users.find(username) != users.end() && 
        users[username].password == oldPassword) {
        
        users[username].password = newPassword;
        saveUsersToFile();
        return true;
    }
    return false;
}

vector<string> LoginSystem::getAllStaffUsernames() {
    vector<string> staffUsers;
    for (const auto& pair : users) {
        if (pair.second.role == UserRole::STAFF) {
            staffUsers.push_back(pair.first);
        }
    }
    return staffUsers;
}

// ----- HÀM LƯU/TẢI NỘI BỘ -----

void LoginSystem::saveUsersToFile() {
    ofstream file(dataFile);
    if (!file.is_open()) return;

    for (const auto& pair : users) {
        const User& user = pair.second;
        file << user.username << "," 
             << user.password << "," 
             << (user.role == UserRole::ADMIN ? "admin" : "staff") << "\n";
    }
    file.close();
}

void LoginSystem::loadUsersFromFile() {
    ifstream file(dataFile);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string username, password, roleStr;
        
        if (getline(ss, username, ',') && 
            getline(ss, password, ',') && 
            getline(ss, roleStr)) {
            
            UserRole role = (roleStr == "admin") ? UserRole::ADMIN : UserRole::STAFF;
            users[username] = {username, password, role};
        }
    }
    file.close();
}