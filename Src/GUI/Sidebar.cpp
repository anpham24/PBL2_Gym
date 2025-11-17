// GUI/Sidebar.cpp
#include "Sidebar.h"
#include "App.h"
#include <iostream>
#include "MainScreen.h"

// Dinh nghia cac muc dieu huong
struct NavItem {
    std::string label;
    ContentScreenType screenType;
    bool adminOnly; // Chi Admin moi thay
};

// --- (CAP NHAT LOGIC QUYEN) ---
// Danh sach tat ca cac muc dieu huong trong he thong
// adminOnly: false = Staff co a thay
// adminOnly: true  = Staff KHONG a thay
const std::vector<NavItem> allNavItems = {
    {"Hoi Vien",    ContentScreenType::HoiVien,   false}, // Staff co the thay
    {"Ban Hang",    ContentScreenType::BanHang,   false}, // Staff co the thay
    {"Goi Tap",     ContentScreenType::GoiTap,    false}, // (MOI) Staff co the thay
    {"Lop Hoc",     ContentScreenType::LopHoc,    false}, // (MOI) Staff co the thay
    {"HLV",         ContentScreenType::HLV,       false}, // (MOI) Staff co the thay
    {"Nhan Vien",   ContentScreenType::NhanVien,  false}, // (MOI) Staff co the thay
    {"Hang Hoa",    ContentScreenType::HangHoa,   false}, // (MOI) Staff co the thay
    {"Bao Cao",     ContentScreenType::BaoCao,    true},  // (VAN AN) Chi Admin
    {"Cai Dat",     ContentScreenType::CaiDat,    false}  // Staff co the thay
};


Sidebar::Sidebar(MainScreen& screen, App& app) 
    : mainScreen(screen), app(app),title(nullptr), staffName(nullptr), activeButton(nullptr), sidebarWidth(220.0f) 
{
    // Thiet lap nen cho Sidebar
    background.setSize(sf::Vector2f(sidebarWidth, (float)app.getWindow().getSize().y));
    background.setFillColor(Config::CardDark); // Mau nen toi
    
    // Thiet lap Title (ten phan mem)
    title = new sf::Text(app.getGlobalFont(), "GYM PRO", 24);
    title->setFillColor(Config::AccentCyan);
    title->setStyle(sf::Text::Bold);
    // SỬA LỖI API: Dùng sf::Vector2f
    title->setPosition(sf::Vector2f(20, 20));

    // --- SỬA STAFFNAME ---
    staffName = new sf::Text(app.getGlobalFont(), "", 16);
    staffName->setFillColor(Config::TextMuted);
    // SỬA LỖI API: Dùng sf::Vector2f
    staffName->setPosition(sf::Vector2f(20, 60));

    // Thiet lap nut Dang Xuat (luon o duoi cung)
    logoutButton.setup("Dang Xuat", app.getGlobalFont());
    logoutButton.setSize(sidebarWidth - 40, 40);
    logoutButton.setPosition(20, app.getWindow().getSize().y - 60.0f);
    logoutButton.setOnClick([this]() {
        this->app.logout();
    });
}

Sidebar::~Sidebar() {
    delete title;
    delete staffName;
}

void Sidebar::setup(Account* currentUser) {
    if (currentUser == nullptr) {
        std::cerr << "Loi Sidebar: Khong co nguoi dung hien tai!" << std::endl;
        return;
    }

    AccountType userType = currentUser->getAccountType();
    
    // Hien thi ten nguoi dung
    if (userType == AccountType::ADMIN) {
        if (staffName) staffName->setString("Xin chao, Admin"); // SỬA
    } else if (currentUser->getLinkedStaff() != nullptr) {
        // ...
        std::string tenNV = currentUser->getLinkedStaff()->getHoTen();
        if (staffName) staffName->setString(sf::String::fromUtf8(tenNV.begin(), tenNV.end())); // SỬA
    } else {
        if (staffName) staffName->setString("Xin chao, Staff"); // SỬA
    }

    navButtons.clear();
    float currentY = 120.0f; 

    // --- LOGIC LOC QUYEN (DA CAP NHAT) ---
    for (const auto& item : allNavItems) {
        
        // Neu muc nay chi danh cho Admin (adminOnly == true)
        // va nguoi dung khong phai la Admin
        if (item.adminOnly && userType != AccountType::ADMIN) {
            continue; // Bo qua, khong tao nut nay (chi xay ra voi "Bao Cao")
        }

        Button btn;
        btn.setup(item.label, app.getGlobalFont());
        btn.setSize(sidebarWidth - 40, 45);
        btn.setPosition(20, currentY);

        btn.setOnClick([this, item]() {
            this->mainScreen.changeContentScreen(item.screenType);
            
            // (Chung ta se them logic de to mau nut active o day)
            // this->activeButton = &btn; 
        });
        
        navButtons.push_back(btn);
        currentY += 55;
    }
}

void Sidebar::handleEvent(sf::Event event, sf::Vector2i mousePos) {
    for (auto& btn : navButtons) {
        btn.handleEvent(event, mousePos);
    }
    logoutButton.handleEvent(event, mousePos);
}

void Sidebar::update(sf::Vector2i mousePos) {
    for (auto& btn : navButtons) {
        btn.update(mousePos);
    }
    logoutButton.update(mousePos);
}

void Sidebar::draw(sf::RenderTarget& target) {
    target.draw(background);
    if (title) target.draw(*title);
    if (staffName) target.draw(*staffName);

    for (auto& btn : navButtons) {
        // (Them logic: Neu nut nay la activeButton -> doi mau)
        btn.draw(target);
    }
    
    logoutButton.draw(target);
}

float Sidebar::getWidth() const {
    return sidebarWidth;
}