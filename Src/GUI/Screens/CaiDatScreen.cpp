// GUI/Screens/CaiDatScreen.cpp
#include "CaiDatScreen.h"
#include "AccountManager.h"
#include <iostream>

// --- Constructor ---
CaiDatScreen::CaiDatScreen(App& app) 
    : BaseScreen(app), 
      isAdmin(false),
        font(app.getGlobalFont()),
      staffSelector(app.getGlobalFont(), "Chon tai khoan Staff..."),
      titleCaNhan(font, "", 22),
      messageCaNhan(font, "", 16),
      titleStaff(font, "", 22),
      messageStaff(font, "", 16)
{
    float contentX = 250.0f;
    Account* currentUser = app.getCurrentAccount();
    
    if (currentUser == nullptr) return; // Khong nen xay ra
    
    isAdmin = (currentUser->getAccountType() == AccountType::ADMIN);

    // --- Thiet lap Khu vuc 1: Doi Mat Khau Ca Nhan ---
    float pY_CaNhan = 100.0f;
    // titleCaNhan.setFont(font);
    titleCaNhan.setString("Doi Mat Khau Ca Nhan");
    // titleCaNhan.setCharacterSize(22);
    titleCaNhan.setFillColor(Config::AccentCyan);
    titleCaNhan.setStyle(sf::Text::Bold);
    titleCaNhan.setPosition(sf::Vector2f(contentX, pY_CaNhan));

    matKhauCuInput.setup("Nhap mat khau cu", font, true);
    matKhauCuInput.setSize(300, 35);
    matKhauCuInput.setPosition(contentX, pY_CaNhan + 50);

    matKhauMoiInput.setup("Nhap mat khau moi", font, true);
    matKhauMoiInput.setSize(300, 35);
    matKhauMoiInput.setPosition(contentX, pY_CaNhan + 100);

    xacNhanMKInput.setup("Xac nhan mat khau moi", font, true);
    xacNhanMKInput.setSize(300, 35);
    xacNhanMKInput.setPosition(contentX, pY_CaNhan + 150);
    
    xacNhanDoiMKButton.setup("Xac Nhan Doi", font);
    xacNhanDoiMKButton.setSize(150, 40);
    xacNhanDoiMKButton.setPosition(contentX, pY_CaNhan + 200);
    xacNhanDoiMKButton.setOnClick([this]() {
        handleChangePassword();
    });
    
    // messageCaNhan.setFont(font);
    // messageCaNhan.setCharacterSize(16);
    messageCaNhan.setPosition(sf::Vector2f(contentX + 170, pY_CaNhan + 210));

    // --- Thiet lap Khu vuc 2: Dat Lai Mat Khau Staff (NEU LA ADMIN) ---
    if (isAdmin) {
        float pY_Staff = pY_CaNhan + 300;
        
        // titleStaff.setFont(font);
        titleStaff.setString("Dat Lai Mat Khau Staff");
        // titleStaff.setCharacterSize(22);
        titleStaff.setFillColor(Config::Warning); // Mau vang
        titleStaff.setStyle(sf::Text::Bold);
        titleStaff.setPosition(sf::Vector2f(contentX, pY_Staff));
        staffSelector.setSize(300, 35);
        staffSelector.setPosition(contentX, pY_Staff + 50);
        
        // Load cac tai khoan Staff vao Selector
        MyVector<Account*> staffAccounts;
        const MyVector<Account*>& allAccounts = app.getAccountManager().getDsAccount();
        for (size_t i = 0; i < allAccounts.size(); ++i) {
            if (allAccounts[i]->getAccountType() == AccountType::STAFF) {
                staffAccounts.push_back(allAccounts[i]);
            }
        }
        staffSelector.setItems(
            staffAccounts,
            [](Account* acc){ return acc->getUsername() + " (" + acc->getLinkedStaff()->getHoTen() + ")"; }
        );

        matKhauMoiStaffInput.setup("Dat mat khau moi cho Staff", font, false); // Hien MK
        matKhauMoiStaffInput.setSize(300, 35);
        matKhauMoiStaffInput.setPosition(contentX, pY_Staff + 100);

        datLaiMKStaffButton.setup("Dat Lai Mat Khau", font);
        datLaiMKStaffButton.setSize(150, 40);
        datLaiMKStaffButton.setPosition(contentX, pY_Staff + 150);
        datLaiMKStaffButton.setOnClick([this]() {
            handleResetPassword();
        });
        
        // messageStaff.setFont(font);
        // messageStaff.setCharacterSize(16);
        messageStaff.setPosition(sf::Vector2f(contentX + 170, pY_Staff + 160));
    }
}

// --- Ham Logic ---

void CaiDatScreen::handleChangePassword() {
    messageCaNhan.setString("");
    Account* currentUser = app.getCurrentAccount();
    
    std::string mkCu = matKhauCuInput.getString();//.toAnsiString();
    std::string mkMoi = matKhauMoiInput.getString();//.toAnsiString();
    std::string xacNhan = xacNhanMKInput.getString();//.toAnsiString();

    // 1. Kiem tra mat khau cu
    if (!currentUser->checkPassword(mkCu)) {
        messageCaNhan.setFillColor(Config::Danger);
        messageCaNhan.setString("Mat khau cu khong dung!");
        return;
    }
    // 2. Kiem tra de trong
    if (mkMoi.empty()) {
        messageCaNhan.setFillColor(Config::Danger);
        messageCaNhan.setString("Mat khau moi khong duoc de trong!");
        return;
    }
    // 3. Kiem tra xac nhan
    if (mkMoi != xacNhan) {
        messageCaNhan.setFillColor(Config::Danger);
        messageCaNhan.setString("Xac nhan mat khau khong khop!");
        return;
    }

    // 4. Thay doi
    bool success = app.getAccountManager().changePassword(currentUser->getUsername(), mkMoi);
    if (success) {
        messageCaNhan.setFillColor(Config::Success);
        messageCaNhan.setString("Doi mat khau thanh cong!");
        // Xoa cac truong input
        matKhauCuInput.setString("");
        matKhauMoiInput.setString("");
        xacNhanMKInput.setString("");
    } else {
        messageCaNhan.setFillColor(Config::Danger);
        messageCaNhan.setString("Loi he thong!");
    }
}

void CaiDatScreen::handleResetPassword() {
    if (!isAdmin) return; // Bao ve them
    messageStaff.setString("");

    Account* staffAcc = staffSelector.getSelected();
    std::string mkMoi = matKhauMoiStaffInput.getString();//.toAnsiString();

    if (staffAcc == nullptr) {
        messageStaff.setFillColor(Config::Danger);
        messageStaff.setString("Vui long chon mot Staff!");
        return;
    }
    if (mkMoi.empty()) {
        messageStaff.setFillColor(Config::Danger);
        messageStaff.setString("Mat khau moi khong duoc de trong!");
        return;
    }

    // 4. Thay doi
    bool success = app.getAccountManager().changePassword(staffAcc->getUsername(), mkMoi);
    if (success) {
        messageStaff.setFillColor(Config::Success);
        messageStaff.setString("Dat lai mat khau cho " + sf::String::fromUtf8(staffAcc->getUsername().begin(), staffAcc->getUsername().end()) + " thanh cong!");
        matKhauMoiStaffInput.setString("");
    } else {
        messageStaff.setFillColor(Config::Danger);
        messageStaff.setString("Loi he thong!");
    }
}

// --- Vong lap chinh ---
void CaiDatScreen::handleEvent(sf::Event event) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(app.getWindow());

    // Khu vuc 1
    matKhauCuInput.handleEvent(event);
    matKhauMoiInput.handleEvent(event);
    xacNhanMKInput.handleEvent(event);
    xacNhanDoiMKButton.handleEvent(event, mousePos);

    // Khu vuc 2 (neu la Admin)
    if (isAdmin) {
        staffSelector.handleEvent(event, mousePos);
        if (!staffSelector.getIsOpen()) { // Chi xu ly input neu dropdown dong
            matKhauMoiStaffInput.handleEvent(event);
            datLaiMKStaffButton.handleEvent(event, mousePos);
        }
    }
    
    // (Bo qua dieu huong phim cho man hinh nay)
}

void CaiDatScreen::update(sf::Time dt) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(app.getWindow());

    // Khu vuc 1
    matKhauCuInput.update(dt);
    matKhauMoiInput.update(dt);
    xacNhanMKInput.update(dt);
    xacNhanDoiMKButton.update(mousePos);

    // Khu vuc 2 (neu la Admin)
    if (isAdmin) {
        staffSelector.update(mousePos);
        matKhauMoiStaffInput.update(dt);
        datLaiMKStaffButton.update(mousePos);
    }
}

void CaiDatScreen::draw(sf::RenderTarget& target) {
    // 1. Ve Khu vuc 1
    target.draw(titleCaNhan);
    matKhauCuInput.draw(target);
    matKhauMoiInput.draw(target);
    xacNhanMKInput.draw(target);
    xacNhanDoiMKButton.draw(target);
    target.draw(messageCaNhan);

    // 2. Ve Khu vuc 2 (neu la Admin)
    if (isAdmin) {
        target.draw(titleStaff);
        matKhauMoiStaffInput.draw(target);
        datLaiMKStaffButton.draw(target);
        target.draw(messageStaff);
        
        // Ve Selector (sau cung de dropdown de len)
        staffSelector.draw(target);
    }
}