
#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

#include "Config.h"
#include "QuanLy.h"
#include "AccountManager.h"
#include "BaseScreen.h"

// Forward declaration
class BaseScreen;

// Dinh nghia cac man hinh chinh cua ung dung
enum class ScreenType {
    Login,
    Main
};

class App {
private:
    sf::RenderWindow window;
    QuanLy& ql;                  // Tham chieu den Singleton QuanLy (data store)
    AccountManager accountManager; // Quan ly logic tai khoan
    
    sf::Font globalFont; // Font chu duoc su dung boi tat ca component
    std::unique_ptr<BaseScreen> currentScreen;
    
    Account* currentAccount; // Tai khoan dang dang nhap (nullptr neu chua dang nhap)
    bool isRunning;

    // Ham khoi tao noi bo
    void loadAssets();
    void initWindow();

public:
    App();
    ~App();

    void run();
    void stop();
    void changeScreen(ScreenType type);
    
    sf::RenderWindow& getWindow();
    sf::Font& getGlobalFont();
    
    QuanLy& getQuanLy();
    AccountManager& getAccountManager();

    void login(Account* acc);
    void logout();
    Account* getCurrentAccount();
};