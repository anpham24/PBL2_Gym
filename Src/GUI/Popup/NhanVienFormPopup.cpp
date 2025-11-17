// GUI/Popups/NhanVienFormPopup.cpp
#include "NhanVienFormPopup.h"
#include "Services/NhanVienService.h" // Goi Service
#include "Managers/AccountManager.h"   // De tao tai khoan
#include "Utils/Validator.h"         // Goi Validator
#include <iostream>

NhanVienFormPopup::NhanVienFormPopup(App& app)
    : BasePopup(app, "Them Nhan Vien"), 
      currentNhanVien(nullptr),
      genderSelector(app.getGlobalFont()),
      focusIndex(0),
      errorMessage(font, "", 14)
{
    // Thiet ke layout (lon hon HoiVienForm)
    sf::Vector2f panelSize(500, 500);
    popupPanel.setSize(panelSize);
    popupPanel.setPosition(sf::Vector2f(
        (app.getWindow().getSize().x - panelSize.x) / 2.0f,
        (app.getWindow().getSize().y - panelSize.y) / 2.0f
    ));
    // Dat lai vi tri title/nut [X]
    title.setPosition(sf::Vector2f(popupPanel.getPosition().x + 20, popupPanel.getPosition().y + 15));
    closeButton.setPosition(popupPanel.getPosition().x + panelSize.x - 40, popupPanel.getPosition().y + 15);
    
    float pX = popupPanel.getPosition().x + 30; // X bat dau (co padding)
    float pY = popupPanel.getPosition().y + 80; // Y bat dau (duoi title)
    float inputWidth = panelSize.x - 180; // Chieu rong input

    // Setup cac InputBox
    hoTenInput.setup("Nhap ho ten", font, false);
    hoTenInput.setSize(inputWidth, 35);
    hoTenInput.setPosition(pX + 120, pY);

    sdtInput.setup("Nhap SDT (VD: 09...)", font, false);
    sdtInput.setSize(inputWidth, 35);
    sdtInput.setPosition(pX + 120, pY + 50);

    ngaySinhInput.setup("DD/MM/YYYY", font, false);
    ngaySinhInput.setSize(inputWidth, 35);
    ngaySinhInput.setPosition(pX + 120, pY + 100);

    // Setup GenderSelector
    genderSelector.setup(pX, pY + 150);

    luongInput.setup("Nhap luong cung (VND)", font, false);
    luongInput.setSize(inputWidth, 35);
    luongInput.setPosition(pX + 120, pY + 200);

    // (MOI) Checkbox cap tai khoan
    capTaiKhoanCheck.setup("Cap tai khoan Staff (Ten dang nhap = ID, Mat khau = SDT)", font);
    capTaiKhoanCheck.setPosition(pX + 120, pY + 250);
    capTaiKhoanCheck.setChecked(true); // Mac dinh la co cap tai khoan

    // Setup Error Message
    // errorMessage.setFont(font);
    // errorMessage.setCharacterSize(14);
    errorMessage.setFillColor(Config::Danger);
    errorMessage.setPosition(sf::Vector2f(pX, pY + 300));

    // Setup Buttons
    float btnY = popupPanel.getPosition().y + panelSize.y - 70;
    confirmButton.setup("Xac Nhan", font);
    confirmButton.setSize(120, 40);
    confirmButton.setPosition(pX + 100, btnY);
    confirmButton.setOnClick([this]() {
        handleSubmit();
    });

    cancelButton.setup("Huy", font);
    cancelButton.setSize(120, 40);
    cancelButton.setPosition(pX + 260, btnY);
    cancelButton.setOnClick([this]() {
        hide();
    });
}

void NhanVienFormPopup::show(NhanVien* nv, std::function<void()> onSuccess) {
    onSuccessCallback = onSuccess;
    errorMessage.setString("");
    
    if (nv == nullptr) {
        // --- THEM MOI ---
        title.setString("Them Nhan Vien Moi");
        currentNhanVien = nullptr;
        // Reset cac truong
        hoTenInput.setString("");
        sdtInput.setString("");
        ngaySinhInput.setString("");
        genderSelector.selectedGender = 0;
        genderSelector.btnNam.setColors(Config::CardLight, Config::CardDark);
        genderSelector.btnNu.setColors(Config::CardLight, Config::CardDark);
        luongInput.setString("");
        capTaiKhoanCheck.setChecked(true);
        // Hien thi checkbox khi them moi
        capTaiKhoanCheck.setPosition(popupPanel.getPosition().x + 120, popupPanel.getPosition().y + 80 + 250);
    } else {
        // --- SUA ---
        title.setString("Cap Nhat Nhan Vien");
        currentNhanVien = nv;
        // Dien thong tin
        hoTenInput.setString(nv->getHoTen());
        sdtInput.setString(nv->getSDT());
        ngaySinhInput.setString(nv->getNgaySinh());
        if (nv->getGioiTinh() == "Nam") genderSelector.btnNam.click();
        else if (nv->getGioiTinh() == "Nu") genderSelector.btnNu.click();
        luongInput.setString(std::to_string((int)nv->getLuong()));
        // An checkbox khi sua
        capTaiKhoanCheck.setPosition(-1000, -1000); // An di
    }
    
    focusIndex = 0;
    updateFocus();
    BasePopup::show();
}

bool NhanVienFormPopup::validate(std::string& ten, std::string& sdt, std::string& gioiTinh, 
                                 std::string& ngaySinh, double& luong)
{
    errorMessage.setString("");
    
    // 1. Lay du lieu tu Form
    ten = hoTenInput.getString();//.toAnsiString(); // SFML 3.0
    sdt = sdtInput.getString();//.toAnsiString();
    gioiTinh = genderSelector.getString();
    ngaySinh = ngaySinhInput.getString();//.toAnsiString(); // Format: DD/MM/YYYY
    
    std::string errorMsg;
    
    // 2. Kiem tra de trong
    if (ten.empty() || sdt.empty() || gioiTinh.empty() || ngaySinh.empty() || luongInput.getString().empty()) {
        errorMessage.setString("Vui long nhap du thong tin");
        return false;
    }

    // 3. Goi Validator cua ban
    errorMsg = Validator::validateTen(ten);
    if (!errorMsg.empty()) { errorMessage.setString(errorMsg); return false; }
    
    errorMsg = Validator::validateSDT(sdt);
    if (!errorMsg.empty()) { errorMessage.setString(errorMsg); return false; }

    errorMsg = Validator::validateNgay(ngaySinh);
    if (!errorMsg.empty()) { errorMessage.setString(errorMsg); return false; }
    
    // 4. Kiem tra Luong
    try {
        luong = std::stod(luongInput.getString());//.toAnsiString());
        errorMsg = Validator::validateSoDuong(luong); // Kiem tra so am
        if (!errorMsg.empty()) { 
            errorMessage.setString("Luong phai la so khong am"); 
            return false; 
        }
    } catch (...) {
        errorMessage.setString("Luong phai la so."); 
        return false;
    }
    
    return true;
}

void NhanVienFormPopup::handleSubmit() {
    std::string ten, sdt, gioiTinh, ngaySinh;
    double luong;

    if (!validate(ten, sdt, gioiTinh, ngaySinh, luong)) {
        return; // Dung lai neu validate loi
    }
    
    if (currentNhanVien == nullptr) {
        // --- THEM MOI ---
        
        // 1. Goi Service de them NhanVien
        // (Luu y: NhanVienService cua ban chua tra ve NhanVien* moi)
        // De hoat dong, ban phai sua NhanVienService::themNhanVien de tra ve NhanVien*
        NhanVienService::themNhanVien(ten, sdt, gioiTinh, ngaySinh, luong);
        
        // 2. Kiem tra checkbox cap tai khoan
        if (capTaiKhoanCheck.getIsChecked()) {
            // Lay lai NhanVien vua tao (thong qua SDT vi no la duy nhat)
            NhanVien* newNV = app.getQuanLy().getNhanVienBySDT(sdt);
            
            if (newNV != nullptr) {
                // 3. Goi AccountManager de them tai khoan
                bool success = app.getAccountManager().addAccount(
                    newNV->getID(),     // Ten dang nhap = ID Nhan Vien
                    sdt,                // Mat khau mac dinh = SDT
                    AccountType::STAFF,
                    newNV               // Lien ket voi doi tuong NhanVien
                );
                
                if (!success) {
                    errorMessage.setString("Loi: Khong the tao tai khoan (ID/Ten TK da ton tai?)");
                    // (O day ban co the can logic rollback - xoa NhanVien vua tao)
                    return; // Dung lai neu loi
                }
            } else {
                 errorMessage.setString("Loi: Khong tim thay NhanVien vua tao de cap TK.");
                 return;
            }
        }
    } else {
        // --- SUA ---
        NhanVienService::suaNhanVien(currentNhanVien->getID(), ten, sdt, gioiTinh, ngaySinh, luong);
    }
    
    if (onSuccessCallback) onSuccessCallback();
    hide();
}

void NhanVienFormPopup::drawContent(sf::RenderTarget& target) {
    // Ve cac label
    sf::Text label(font, "", 16);
    label.setFillColor(Config::TextNormal);
    float pX = popupPanel.getPosition().x + 30;
    float pY = popupPanel.getPosition().y + 80;
    
    label.setString("Ho Ten:");
    label.setPosition(sf::Vector2f(pX, pY + 5)); target.draw(label);
    label.setString("SDT:");
    label.setPosition(sf::Vector2f(pX, pY + 55)); target.draw(label);
    label.setString("Ngay Sinh:");
    label.setPosition(sf::Vector2f(pX, pY + 105)); target.draw(label);
    label.setString("Luong Cung:");
    label.setPosition(sf::Vector2f(pX, pY + 205)); target.draw(label);

    // Ve cac component
    hoTenInput.draw(target);
    sdtInput.draw(target);
    ngaySinhInput.draw(target);
    genderSelector.draw(target);
    luongInput.draw(target);
    
    if (currentNhanVien == nullptr) { // Chi hien checkbox khi them moi
        capTaiKhoanCheck.draw(target);
    }
    
    target.draw(errorMessage);
    confirmButton.draw(target);
    cancelButton.draw(target);
}

void NhanVienFormPopup::handleKeyNavigation(sf::Keyboard::Key key) {
    // 0:Ten, 1:SDT, 2:NgaySinh, 3:GenderNam, 4:GenderNu, 5:Luong, 6:Checkbox, 7:Confirm, 8:Cancel
    int maxFocus = (currentNhanVien == nullptr) ? 8 : 7; // Bo qua checkbox khi Sua
    
    if (key == sf::Keyboard::Key::Tab || key == sf::Keyboard::Key::Down) {
        focusIndex = (focusIndex + 1) % (maxFocus + 1);
    } 
    else if (key == sf::Keyboard::Key::Up) {
        focusIndex = (focusIndex - 1 + (maxFocus + 1)) % (maxFocus + 1);
    }
    else if (key == sf::Keyboard::Key::Enter) {
        if (focusIndex < 2) focusIndex++; // 0->1, 1->2
        else if (focusIndex == 2) focusIndex = 5; // Tu NgaySinh -> Luong
        else if (focusIndex == 3) genderSelector.btnNam.click();
        else if (focusIndex == 4) genderSelector.btnNu.click();
        else if (focusIndex == 5) focusIndex = (currentNhanVien == nullptr) ? 6 : 7; // Tu Luong -> Checkbox hoac Confirm
        else if (focusIndex == 6) capTaiKhoanCheck.toggle();
        else if (focusIndex == 7) confirmButton.click();
        else if (focusIndex == 8) cancelButton.click();
    }
    updateFocus();
}

void NhanVienFormPopup::updateFocus() {
    hoTenInput.setFocus(focusIndex == 0);
    sdtInput.setFocus(focusIndex == 1);
    ngaySinhInput.setFocus(focusIndex == 2);
    genderSelector.setFocused(focusIndex == 3, focusIndex == 4);
    luongInput.setFocus(focusIndex == 5);
    capTaiKhoanCheck.setFocused(focusIndex == 6);
    confirmButton.setFocused(focusIndex == 7);
    cancelButton.setFocused(focusIndex == 8);
}

void NhanVienFormPopup::handleEvent(sf::Event event, sf::Vector2i mousePos) {
    if (!isVisible) return;
    BasePopup::handleEvent(event, mousePos); // Nut [X]

    confirmButton.handleEvent(event, mousePos);
    cancelButton.handleEvent(event, mousePos);
    genderSelector.handleEvent(event, mousePos);
    if (currentNhanVien == nullptr) {
        capTaiKhoanCheck.handleEvent(event, mousePos);
    }
    
    // Xu ly input (TextEntered)
    if (event.getIf<sf::Event::TextEntered>()) {
        if (focusIndex == 0) hoTenInput.handleEvent(event);
        else if (focusIndex == 1) sdtInput.handleEvent(event);
        else if (focusIndex == 2) ngaySinhInput.handleEvent(event);
        else if (focusIndex == 5) luongInput.handleEvent(event);
    }
    
    // Xu ly dieu huong
    if (auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        if (focusIndex == 0) hoTenInput.handleEvent(event);
        else if (focusIndex == 1) sdtInput.handleEvent(event);
        else if (focusIndex == 2) ngaySinhInput.handleEvent(event);
        else if (focusIndex == 5) luongInput.handleEvent(event);
        
        handleKeyNavigation(keyEvent->code);
    }
}

void NhanVienFormPopup::update(sf::Vector2i mousePos) {
    if (!isVisible) return;
    BasePopup::update(mousePos); // Nut [X]

    hoTenInput.update(sf::Time::Zero);
    sdtInput.update(sf::Time::Zero);
    ngaySinhInput.update(sf::Time::Zero);
    luongInput.update(sf::Time::Zero);
    
    genderSelector.update(mousePos);
    if (currentNhanVien == nullptr) {
        capTaiKhoanCheck.update(mousePos);
    }
    confirmButton.update(mousePos);
    cancelButton.update(mousePos);
}