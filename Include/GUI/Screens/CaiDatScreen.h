// GUI/Screens/CaiDatScreen.h
#pragma once

#include "BaseScreen.h"
#include "App.h"
#include "InputBox.h"
#include "Button.h"
#include "Selector.h"
#include "Account.h"

/*
 * Lop CaiDatScreen cho phep nguoi dung thay doi mat khau.
 * Neu la Admin, se co them chuc nang dat lai mat khau cho Staff.
 */
class CaiDatScreen : public BaseScreen {
private:
    bool isAdmin; // Luu quyen cua nguoi dung

    // --- Khu vuc 1: Doi Mat Khau Ca Nhan ---
    sf::Font& font;
    sf::Text titleCaNhan;
    InputBox matKhauCuInput;
    InputBox matKhauMoiInput;
    InputBox xacNhanMKInput;
    Button xacNhanDoiMKButton;
    sf::Text messageCaNhan; // De hien thi loi hoac thanh cong

    // --- Khu vuc 2: Dat Lai Mat Khau Staff (Chi Admin thay) ---
    sf::Text titleStaff;
    Selector<Account> staffSelector; // De chon Staff
    InputBox matKhauMoiStaffInput;
    Button datLaiMKStaffButton;
    sf::Text messageStaff; // De hien thi loi hoac thanh cong

    // Ham helper
    void handleChangePassword();
    void handleResetPassword();

public:
    CaiDatScreen(App& app);
    
    void handleEvent(sf::Event event) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderTarget& target) override;
};