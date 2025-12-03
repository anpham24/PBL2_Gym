// GUI/Sidebar.cpp
#include "Sidebar.h"
#include "Screens/MainScreen.h"
#include "../App.h"
#include <iostream>
#include <string>

// Dinh nghia cac muc dieu huong
struct NavItem
{
    std::string label;
    ContentScreenType screenType;
    bool adminOnly; // Chi Admin moi thay
};

// (CAP NHAT LOGIC QUYEN)
const std::vector<NavItem> allNavItems = {
    {"Hoi Vien", ContentScreenType::HoiVien, false},
    {"Ban Hang", ContentScreenType::BanHang, false},
    {"Goi Tap", ContentScreenType::GoiTap, false},
    {"Lop Hoc", ContentScreenType::LopHoc, false},
    {"HLV", ContentScreenType::HLV, false},
    {"Nhan Vien", ContentScreenType::NhanVien, false},
    {"Hang Hoa", ContentScreenType::HangHoa, false},
    {"Mon Tap", ContentScreenType::MonTap, false},
    {"Bao Cao", ContentScreenType::BaoCao, true},
    {"Cai Dat", ContentScreenType::CaiDat, false}};

Sidebar::Sidebar(MainScreen &screen, App &app)
    : mainScreen(screen), app(app), title(nullptr), staffName(nullptr), activeButton(nullptr), sidebarWidth(220.0f)
{
    background.setSize(sf::Vector2f(sidebarWidth, (float)app.getWindow().getSize().y));
    background.setFillColor(Config::CardDark);

    // SỬA: Dùng 'new' để khởi tạo con trỏ
    title = new sf::Text(app.getGlobalFont(), "GYM PRO", 24);

    // SỬA: Dùng '->' thay vì '.'
    title->setFillColor(Config::AccentCyan);
    title->setStyle(sf::Text::Bold);
    title->setPosition(sf::Vector2f(20, 20));

    // SỬA: Dùng 'new'
    staffName = new sf::Text(app.getGlobalFont(), "", 16);

    // SỬA: Dùng '->' thay vì '.'
    staffName->setFillColor(Config::TextMuted);
    staffName->setPosition(sf::Vector2f(20, 60));

    logoutButton.setup("Dang Xuat", app.getGlobalFont());
    logoutButton.setSize(sidebarWidth - 40, 40);
    logoutButton.setPosition(20, app.getWindow().getSize().y - 60.0f);
    logoutButton.setOnClick([this]()
                            { this->app.logout(); });
}

Sidebar::~Sidebar()
{
    delete title;
    delete staffName;
}

void Sidebar::setup(Account *currentUser)
{
    if (currentUser == nullptr)
        return;

    AccountType userType = currentUser->getAccountType();

    // Hien thi ten nguoi dung
    if (userType == AccountType::ADMIN)
    {
        // SỬA: Dùng '->'
        if (staffName)
            staffName->setString("Xin chao, Manager");
    }
    else if (currentUser->getLinkedStaff() != nullptr)
    {
        std::string tenNV = currentUser->getLinkedStaff()->getHoTen();
        // SỬA: Dùng '->'
        if (staffName)
            staffName->setString("NV: " + sf::String::fromUtf8(tenNV.begin(), tenNV.end()));
    }
    else
    {
        // SỬA: Dùng '->'
        if (staffName)
            staffName->setString("Xin chao, Staff (Loi)");
    }

    navButtons.clear();
    float currentY = 120.0f;

    // Logic loc quyen
    for (const auto &item : allNavItems)
    {
        if (item.adminOnly && userType != AccountType::ADMIN)
        {
            continue;
        }

        Button btn;
        btn.setup(item.label, app.getGlobalFont());
        btn.setSize(sidebarWidth - 40, 45);
        btn.setPosition(20, currentY);

        btn.setOnClick([this, item]()
                       { this->mainScreen.changeContentScreen(item.screenType); });

        navButtons.push_back(btn);
        currentY += 55;
    }
}

void Sidebar::handleEvent(sf::Event event, sf::Vector2i mousePos)
{
    for (auto &btn : navButtons)
    {
        btn.handleEvent(event, mousePos);
    }
    logoutButton.handleEvent(event, mousePos);
}

void Sidebar::update(sf::Vector2i mousePos)
{
    for (auto &btn : navButtons)
    {
        btn.update(mousePos);
    }
    logoutButton.update(mousePos);
}

void Sidebar::draw(sf::RenderTarget &target)
{
    target.draw(background);

    // SỬA: Dùng '*' để vẽ đối tượng mà con trỏ trỏ tới
    if (title)
        target.draw(*title);
    if (staffName)
        target.draw(*staffName);

    for (auto &btn : navButtons)
    {
        btn.draw(target);
    }

    logoutButton.draw(target);
}

float Sidebar::getWidth() const
{
    return sidebarWidth;
}