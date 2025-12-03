// App.cpp
#include "App.h"
#include "LoginScreen.h"
#include "MainScreen.h"
#include "FileIO.h" // <-- BAO GOM FILEIO
#include <iostream>

App::App()
    : ql(QuanLy::getInstance()),
      currentAccount(nullptr),
      isRunning(true)
{
    initWindow();
    loadAssets();

    // --- (MOI) LOGIC TAI DU LIEU VA TAO ADMIN ---
    try
    {
        FileIO::loadAllData(ql, accountManager, "./data");
    }
    catch (...)
    {
        std::cerr << "Loi: Khong the loadAllData. Co the do FileIO cu." << std::endl;
    }

    if (accountManager.getAccount("Manager") == nullptr)
    {
        std::cout << "Khong tim thay 'Manager', dang tao moi voi mat khau '123'..." << std::endl;
        accountManager.addAccount("Manager", "123", AccountType::ADMIN, nullptr);
        ql.setDirty(true); // De luu tai khoan admin moi nay khi thoat
    }

    // Bat dau ung dung tai man hinh Login
    changeScreen(ScreenType::Login);
}

App::~App()
{
    // --- (MOI) LUU DU LIEU KHI THOAT ---
    if (ql.getIsDirty())
    {
        std::cout << "Dang luu thay doi truoc khi thoat..." << std::endl;
        try
        {
            FileIO::saveAllData(ql, accountManager, "./data");
        }
        catch (...)
        {
            std::cerr << "Loi: Khong the saveAllData. Co the do FileIO cu." << std::endl;
        }
    }
    else
    {
        std::cout << "Khong co thay doi, thoat." << std::endl;
    }
}

// ... (Cac ham initWindow, loadAssets, run, stop, changeScreen...
//      giu nguyen nhu file toi da gui ban truoc do) ...

void App::initWindow()
{
    window.create(
        sf::VideoMode({Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT}),
        "He Thong Quan Ly Gym (GYM PRO)",
        sf::Style::Default);
    window.setFramerateLimit(60);
}

void App::loadAssets()
{
    if (!globalFont.openFromFile(Config::FONT_PATH))
    {
        std::cerr << "Loi: Khong the tai font tai: " << Config::FONT_PATH << std::endl;
        isRunning = false;
        window.close();
    }
}

void App::run()
{
    sf::Clock clock;
    while (isRunning && window.isOpen())
    {
        sf::Time dt = clock.restart();

        // sf::Event event;
        while (auto event = window.pollEvent())
        {
            // *** SFML 3.0 API CHANGE ***
            if (event->is<sf::Event::Closed>())
            {
                isRunning = false;
                window.close();
            }

            if (currentScreen)
            {
                currentScreen->handleEvent(*event);
            }
        }

        if (currentScreen)
        {
            currentScreen->update(dt);
        }

        window.clear(Config::NavyBackground);

        if (currentScreen)
        {
            currentScreen->draw(window);
        }

        window.display();
    }
}

void App::stop()
{
    isRunning = false;
    window.close();
}

void App::changeScreen(ScreenType type)
{
    currentScreen.reset();

    if (type == ScreenType::Login)
    {
        currentScreen = std::make_unique<LoginScreen>(*this);
    }
    else if (type == ScreenType::Main)
    {
        if (currentAccount == nullptr)
        {
            std::cerr << "Loi: Co gang vao MainScreen ma khong dang nhap!" << std::endl;
            changeScreen(ScreenType::Login); // Quay lai Login
        }
        else
        {
            currentScreen = std::make_unique<MainScreen>(*this);
        }
    }
}

sf::RenderWindow &App::getWindow()
{
    return window;
}

sf::Font &App::getGlobalFont()
{
    return globalFont;
}

QuanLy &App::getQuanLy()
{
    return ql;
}

AccountManager &App::getAccountManager()
{
    return accountManager;
}

void App::login(Account *acc)
{
    currentAccount = acc;
    std::cout << "Nguoi dung da dang nhap: " << acc->getUsername() << std::endl;
}

void App::logout()
{
    currentAccount = nullptr;
    changeScreen(ScreenType::Login);
    std::cout << "Nguoi dung da dang xuat." << std::endl;
}

Account *App::getCurrentAccount()
{
    return currentAccount;
}