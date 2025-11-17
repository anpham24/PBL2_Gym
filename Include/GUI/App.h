// App.h
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

/*
 * Lop App la lop chinh cua toan bo ung dung.
 * No quan ly cua so (window), vong lap chinh (run loop),
 * va dieu huong giua cac man hinh (vi du: tu LoginScreen sang MainScreen).
 */
class App {
private:
    sf::RenderWindow window;
    QuanLy& ql;                   // Tham chieu den Singleton QuanLy (data store)
    AccountManager accountManager; // Quan ly logic tai khoan
    
    sf::Font globalFont; // Font chu duoc su dung boi tat ca component
    
    // Con tro thong minh de quan ly man hinh hien tai
    std::unique_ptr<BaseScreen> currentScreen;
    
    Account* currentAccount; // Tai khoan dang dang nhap (nullptr neu chua dang nhap)
    bool isRunning;

    // Ham khoi tao noi bo
    void loadAssets();
    void initWindow();

public:
    App();
    ~App();

    // Vong lap chinh cua ung dung
    void run();
    
    // Ngat vong lap va dong ung dung
    void stop();

    /**
     * @brief Ham dieu huong chinh, chuyen doi giua cac man hinh (Login/Main).
     * @param type Loai man hinh de chuyen den.
     */
    void changeScreen(ScreenType type);
    
    // --- Cac ham truy cap toan cuc ---

    /**
     * @brief Tra ve cua so chinh de cac component tuong tac.
     */
    sf::RenderWindow& getWindow();
    
    /**
     * @brief Tra ve font chu toan cuc.
     */
    sf::Font& getGlobalFont();
    
    /**
     * @brief Tra ve tham chieu den kho du lieu QuanLy.
     */
    QuanLy& getQuanLy();
    
    /**
     * @brief Tra ve tham chieu den trinh quan ly tai khoan.
     */
    AccountManager& getAccountManager();

    // --- Quan ly Phien Dang Nhap ---

    /**
     * @brief Luu lai tai khoan vua dang nhap thanh cong.
     * @param acc Con tro den tai khoan.
     */
    void login(Account* acc);
    
    /**
     * @brief Dang xuat va quay ve man hinh Login.
     */
    void logout();
    
    /**
     * @brief Lay tai khoan dang dang nhap hien tai.
     * @return Con tro den Account, hoac nullptr neu chua dang nhap.
     */
    Account* getCurrentAccount();
};