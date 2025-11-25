// GUI/Screens/CaiDatScreen.cpp
#include "CaiDatScreen.h"
#include "AccountManager.h"
#include <iostream>

// --- Constructor ---
CaiDatScreen::CaiDatScreen(App& app) 
    : BaseScreen(app), 
      font(app.getGlobalFont()),
      isAdmin(false),
      staffSelector(app.getGlobalFont(), "Chon tai khoan Staff..."),
      titleCaNhan(font, "", 22),
      messageCaNhan(font, "", 16),
      titleStaff(font, "", 22),
      messageStaff(font, "", 16),
      // Khoi tao InputBox va Button
      matKhauCuInput(),
      matKhauMoiInput(),
      xacNhanMKInput(),
      xacNhanDoiMKButton(),
      matKhauMoiStaffInput(),
      datLaiMKStaffButton(),
      focusIndex(0) // Mac dinh focus vao ô dau tien
{
    float contentX = 250.0f;
    Account* currentUser = app.getCurrentAccount();
    
    if (currentUser == nullptr) return; // Khong nen xay ra
    
    isAdmin = (currentUser->getAccountType() == AccountType::ADMIN);

    // --- Thiet lap Khu vuc 1: Doi Mat Khau Ca Nhan ---
    float pY_CaNhan = 100.0f;

    titleCaNhan.setString("Doi Mat Khau Ca Nhan");
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
    
    messageCaNhan.setPosition(sf::Vector2f(contentX + 170, pY_CaNhan + 210));

    // --- Thiet lap Khu vuc 2: Dat Lai Mat Khau Staff (NEU LA ADMIN) ---
    if (isAdmin) {
        float pY_Staff = pY_CaNhan + 300;
        
        titleStaff.setString("Dat Lai Mat Khau Staff");
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
        
        messageStaff.setPosition(sf::Vector2f(contentX + 170, pY_Staff + 160));
    }
    
    // Thiet lap focus ban dau
    updateFocus();
}

// --- (MOI) HAM QUAN LY FOCUS ---
void CaiDatScreen::updateFocus() {
    // Khu vuc 1
    matKhauCuInput.setFocus(focusIndex == 0);
    matKhauMoiInput.setFocus(focusIndex == 1);
    xacNhanMKInput.setFocus(focusIndex == 2);
    xacNhanDoiMKButton.setFocused(focusIndex == 3);

    // Khu vuc 2 (Admin)
    if (isAdmin) {
        // Selector khong co setFocus truc quan, bo qua
        matKhauMoiStaffInput.setFocus(focusIndex == 5);
        datLaiMKStaffButton.setFocused(focusIndex == 6);
    }
}

void CaiDatScreen::handleKeyNavigation(sf::Keyboard::Key key) {
    int maxFocus = isAdmin ? 6 : 3;
    
    if (key == sf::Keyboard::Key::Tab || key == sf::Keyboard::Key::Down) {
        focusIndex = (focusIndex + 1) % (maxFocus + 1);
        updateFocus();
    } 
    else if (key == sf::Keyboard::Key::Up) {
        focusIndex = (focusIndex - 1 + (maxFocus + 1)) % (maxFocus + 1);
        updateFocus();
    }
    else if (key == sf::Keyboard::Key::Enter) {
        if (focusIndex == 3) { // Nut Xac Nhan Doi MK
            xacNhanDoiMKButton.click();
        } else if (focusIndex == 6 && isAdmin) { // Nut Reset MK Staff
            datLaiMKStaffButton.click();
        } else {
            // Neu dang o input, Enter se chuyen xuong o duoi
            focusIndex = (focusIndex + 1) % (maxFocus + 1);
            updateFocus();
        }
    }
}

// --- Ham Logic ---

void CaiDatScreen::handleChangePassword() {
    messageCaNhan.setString("");
    Account* currentUser = app.getCurrentAccount();
    
    std::string mkCu = matKhauCuInput.getString();
    std::string mkMoi = matKhauMoiInput.getString();
    std::string xacNhan = xacNhanMKInput.getString();

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
        app.getQuanLy().setDirty(true);
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
    std::string mkMoi = matKhauMoiStaffInput.getString();

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
        app.getQuanLy().setDirty(true);
        matKhauMoiStaffInput.setString("");
    } else {
        messageStaff.setFillColor(Config::Danger);
        messageStaff.setString("Loi he thong!");
    }
}

// --- Vong lap chinh ---
void CaiDatScreen::handleEvent(sf::Event event) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(app.getWindow());

    // 1. Xu ly Click chuot de Focus (QUAN TRONG)
    if (auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            if (matKhauCuInput.isMouseOver(mousePos)) focusIndex = 0;
            else if (matKhauMoiInput.isMouseOver(mousePos)) focusIndex = 1;
            else if (xacNhanMKInput.isMouseOver(mousePos)) focusIndex = 2;
            // ... (Button 3 tu xu ly click)
            
            if (isAdmin) {
                // Index 4 la Selector (tu xu ly)
                if (matKhauMoiStaffInput.isMouseOver(mousePos)) focusIndex = 5;
            }
            updateFocus();
        }
    }

    // 2. Xu ly ban phim dieu huong
    if (auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        handleKeyNavigation(keyEvent->code);
    }

    // 3. Truyen su kien cho cac Component
    matKhauCuInput.handleEvent(event);
    matKhauMoiInput.handleEvent(event);
    xacNhanMKInput.handleEvent(event);
    xacNhanDoiMKButton.handleEvent(event, mousePos);

    // Khu vuc 2 (neu la Admin)
    if (isAdmin) {
        staffSelector.handleEvent(event, mousePos);
        // Neu dropdown dong thi moi cho nhap o duoi
        if (!staffSelector.getIsOpen()) { 
            matKhauMoiStaffInput.handleEvent(event);
            datLaiMKStaffButton.handleEvent(event, mousePos);
        }
    }
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