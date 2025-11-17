// GUI/Popups/HoiVienFormPopup.cpp
#include "HoiVienFormPopup.h"
#include "HoiVienService.h" // Goi Service
#include "Validator.h"         // Goi Validator
#include <iostream>

// ========================================================================
// IMPLEMENTATION cho GenderSelector (Component con)
// ========================================================================
GenderSelector::GenderSelector(sf::Font& font) 
    : font(font), selectedGender(0), label(font,"Gioi tinh:", 16), btnNam(), btnNu()
{
    // label.setFont(font);
    // label.setString("Gioi tinh:");
    // label.setCharacterSize(16);
    label.setFillColor(Config::TextNormal);
    
    btnNam.setup("Nam", font);
    btnNu.setup("Nu", font);
    btnNam.setSize(80, 35);
    btnNu.setSize(80, 35);
    
    // Dat mau mac dinh cho nut chua chon
    btnNam.setColors(Config::CardLight, Config::CardDark);
    btnNu.setColors(Config::CardLight, Config::CardDark);
    
    // Khi click, thay doi trang thai va mau sac
    btnNam.setOnClick([this](){ 
        selectedGender = 1; 
        btnNam.setColors(Config::AccentCyan, Config::AccentCyan); // Mau chon
        btnNu.setColors(Config::CardLight, Config::CardDark); // Mau mac dinh
    });
    btnNu.setOnClick([this](){ 
        selectedGender = 2; 
        btnNu.setColors(Config::AccentCyan, Config::AccentCyan); // Mau chon
        btnNam.setColors(Config::CardLight, Config::CardDark); // Mau mac dinh
    });
}

void GenderSelector::setup(float x, float y) {
    label.setPosition(sf::Vector2f(x, y + 5));
    btnNam.setPosition(x + 120, y);
    btnNu.setPosition(x + 210, y);
}

void GenderSelector::draw(sf::RenderTarget& target) {
    target.draw(label);
    btnNam.draw(target);
    btnNu.draw(target);
}

void GenderSelector::handleEvent(sf::Event event, sf::Vector2i mousePos) {
    btnNam.handleEvent(event, mousePos);
    btnNu.handleEvent(event, mousePos);
}

void GenderSelector::update(sf::Vector2i mousePos) {
    btnNam.update(mousePos);
    btnNu.update(mousePos);
}

std::string GenderSelector::getString() {
    if (selectedGender == 1) return "Nam";
    if (selectedGender == 2) return "Nu";
    return "";
}

void GenderSelector::setFocused(bool focusNam, bool focusNu) {
    btnNam.setFocused(focusNam);
    btnNu.setFocused(focusNu);
}


// ========================================================================
// IMPLEMENTATION cho HoiVienFormPopup
// ========================================================================
HoiVienFormPopup::HoiVienFormPopup(App& app)
    : BasePopup(app, "Them Hoi Vien Moi"),
      currentHoiVien(nullptr),
      genderSelector(app.getGlobalFont()),
      focusIndex(0),
      errorMessage(font, "", 14),
      hoTenInput(),         // Gọi InputBox::InputBox() (đã sửa)
      sdtInput(),           // Gọi InputBox::InputBox() (đã sửa)
      ngaySinhInput(),      // Gọi InputBox::InputBox() (đã sửa)
    //   errorMessage(font, "", 14), // <--- Quan trọng!
      confirmButton(),      // Gọi Button::Button() (đã sửa)
      cancelButton()        // Gọi Button::Button() (đã sửa)
{
    // Thiet ke layout
    sf::Vector2f panelSize(500, 450);
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

    // Setup Error Message
    // errorMessage.setFont(font);
    // errorMessage.setCharacterSize(14);
    errorMessage.setFillColor(Config::Danger);
    errorMessage.setPosition(sf::Vector2f(pX, pY + 200));

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

void HoiVienFormPopup::show(HoiVien* hv, std::function<void()> onSuccess) {
    onSuccessCallback = onSuccess; // Luu lai ham callback
    errorMessage.setString("");
    
    if (hv == nullptr) {
        // --- THEM MOI ---
        title.setString("Them Hoi Vien Moi");
        currentHoiVien = nullptr;
        
        // Xoa het du lieu cu
        hoTenInput.setString("");
        sdtInput.setString("");
        ngaySinhInput.setString("");
        genderSelector.selectedGender = 0;
        genderSelector.btnNam.setColors(Config::CardLight, Config::CardDark);
        genderSelector.btnNu.setColors(Config::CardLight, Config::CardDark);
        
    } else {
        // --- SUA ---
        title.setString("Cap Nhat Thong Tin");
        currentHoiVien = hv;
        
        // Dien thong tin cua hoi vien
        hoTenInput.setString(hv->getHoTen());
        sdtInput.setString(hv->getSDT());
        ngaySinhInput.setString(hv->getNgaySinh()); // Gia su format la DD/MM/YYYY
        
        if (hv->getGioiTinh() == "Nam") {
            genderSelector.btnNam.click();
        } else if (hv->getGioiTinh() == "Nu") {
            genderSelector.btnNu.click();
        }
    }
    
    focusIndex = 0;
    updateFocus();
    BasePopup::show();
}

void HoiVienFormPopup::drawContent(sf::RenderTarget& target) {
    // Ve cac label
    sf::Text label(font, sf::String(), 16);
    label.setFillColor(Config::TextNormal);
    float pX = popupPanel.getPosition().x + 30;
    float pY = popupPanel.getPosition().y + 80;
    
    label.setString("Ho Ten:");
    label.setPosition(sf::Vector2f(pX, pY + 5));
    target.draw(label);

    label.setString("SDT:");
    label.setPosition(sf::Vector2f(pX, pY + 55));
    target.draw(label);

    label.setString("Ngay Sinh:");
    label.setPosition(sf::Vector2f(pX, pY + 105));
    target.draw(label);

    // Ve cac component
    hoTenInput.draw(target);
    sdtInput.draw(target);
    ngaySinhInput.draw(target);
    genderSelector.draw(target);
    
    target.draw(errorMessage);
    
    confirmButton.draw(target);
    cancelButton.draw(target);
}

void HoiVienFormPopup::handleEvent(sf::Event event, sf::Vector2i mousePos) {
    if (!isVisible) return;
    BasePopup::handleEvent(event, mousePos); // Xu ly nut [X]

    // Truyen event cho cac component con
    confirmButton.handleEvent(event, mousePos);
    cancelButton.handleEvent(event, mousePos);
    genderSelector.handleEvent(event, mousePos);

    // Xu ly input (TextEntered)
    if (event.getIf<sf::Event::TextEntered>()) {
        if (focusIndex == 0) hoTenInput.handleEvent(event);
        else if (focusIndex == 1) sdtInput.handleEvent(event);
        else if (focusIndex == 2) ngaySinhInput.handleEvent(event);
    }
    
    // Xu ly dieu huong
    if (auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        // Truyen phim dieu huong (Left, Right, Delete...) cho input
        if (focusIndex == 0) hoTenInput.handleEvent(event);
        else if (focusIndex == 1) sdtInput.handleEvent(event);
        else if (focusIndex == 2) ngaySinhInput.handleEvent(event);

        // Dieu huong (Tab, Down, Up, Enter)
        handleKeyNavigation(keyEvent->code);
    }
}

void HoiVienFormPopup::updateFocus() {
    hoTenInput.setFocus(focusIndex == 0);
    sdtInput.setFocus(focusIndex == 1);
    ngaySinhInput.setFocus(focusIndex == 2);
    genderSelector.setFocused(focusIndex == 3, focusIndex == 4);
    confirmButton.setFocused(focusIndex == 5);
    cancelButton.setFocused(focusIndex == 6);
}

void HoiVienFormPopup::handleKeyNavigation(sf::Keyboard::Key key) {
    int maxFocus = 6;
    if (key == sf::Keyboard::Key::Tab || key == sf::Keyboard::Key::Down) {
        focusIndex = (focusIndex + 1) % (maxFocus + 1);
        updateFocus();
    } 
    else if (key == sf::Keyboard::Key::Up) {
        focusIndex = (focusIndex - 1 + (maxFocus + 1)) % (maxFocus + 1);
        updateFocus();
    }
    else if (key == sf::Keyboard::Key::Enter) {
        switch (focusIndex) {
            case 0: // Tu Ho Ten -> SDT
            case 1: // Tu SDT -> Ngay Sinh
            case 2: // Tu Ngay Sinh -> Gioi Tinh (Nam)
                focusIndex++;
                updateFocus();
                break;
            case 3: // O nut Nam
                genderSelector.btnNam.click();
                break;
            case 4: // O nut Nu
                genderSelector.btnNu.click();
                break;
            case 5: // O nut Xac Nhan
                confirmButton.click();
                break;
            case 6: // O nut Huy
                cancelButton.click();
                break;
        }
    }
}

void HoiVienFormPopup::update(sf::Vector2i mousePos) {
    if (!isVisible) return;
    BasePopup::update(mousePos); // Xu ly nut [X]

    hoTenInput.update(sf::Time::Zero); // Update dau nhay
    sdtInput.update(sf::Time::Zero);
    ngaySinhInput.update(sf::Time::Zero);
    
    genderSelector.update(mousePos);
    confirmButton.update(mousePos);
    cancelButton.update(mousePos);
}

bool HoiVienFormPopup::validateInfo(std::string& ten, std::string& sdt, std::string& gioiTinh, std::string& ngaySinh) {
    errorMessage.setString(""); // Xoa loi cu
    
    // 1. Lay du lieu tu Form
    ten = hoTenInput.getString();
    sdt = sdtInput.getString();
    gioiTinh = genderSelector.getString();
    ngaySinh = ngaySinhInput.getString();

    // 2. Kiem tra de trong (theo yeu cau)
    if (ten.empty() || sdt.empty() || gioiTinh.empty() || ngaySinh.empty()) {
        errorMessage.setString("Vui long nhap du thong tin");
        // (Them logic boi vien do o day)
        return false;
    }

    // 3. Kiem tra tinh hop le (goi Validator cua ban)
    std::string errorMsg;
    
    errorMsg = Validator::validateTen(ten);
    if (!errorMsg.empty()) { errorMessage.setString(errorMsg); return false; }
    
    errorMsg = Validator::validateSDT(sdt);
    if (!errorMsg.empty()) { errorMessage.setString(errorMsg); return false; }

    // (validateGioiTinh da duoc dam bao boi GenderSelector)

    errorMsg = Validator::validateNgay(ngaySinh);
    if (!errorMsg.empty()) { errorMessage.setString(errorMsg); return false; }

    return true;
}

void HoiVienFormPopup::handleSubmit() {
    std::string ten, sdt, gioiTinh, ngaySinh;
    
    if (!validateInfo(ten, sdt, gioiTinh, ngaySinh)) {
        return; // Neu validate that bai, dung lai
    }

    if (currentHoiVien == nullptr) {
        // --- THEM MOI ---
        // Goi Service voi cac tham so da duoc validate
        // (Mac dinh point = 0 khi them moi)
        HoiVienService::themHoiVien(ten, sdt, gioiTinh, ngaySinh, 0);
    } else {
        // --- CAP NHAT ---
        // (Service sua cua ban co point, nen ta lay point cu)
        HoiVienService::suaHoiVien(
            currentHoiVien->getID(), 
            ten, 
            sdt, 
            gioiTinh, 
            ngaySinh, 
            currentHoiVien->getPoint()
        );
    }
    
    // (Kiem tra loi tu Service - Hien tai Service dang tu show loi)
    
    if (onSuccessCallback) {
        onSuccessCallback(); // Goi callback de load lai bang
    }
    
    hide(); // Dong popup
}