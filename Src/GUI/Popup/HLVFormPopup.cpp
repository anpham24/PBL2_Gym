// GUI/Popups/HLVFormPopup.cpp
#include "HLVFormPopup.h"
#include "HLVService.h" // Goi Service
#include "Validator.h"   // Goi Validator

HLVFormPopup::HLVFormPopup(App& app)
    : BasePopup(app, "Them HLV Moi"), 
      currentHLV(nullptr),
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
    
    chuyenMonInput.setup("Nhap chuyen mon (VD: Yoga, Gym...)", font, false);
    chuyenMonInput.setSize(inputWidth, 35);
    chuyenMonInput.setPosition(pX + 120, pY + 200);

    luongInput.setup("Nhap luong cung (VND)", font, false);
    luongInput.setSize(inputWidth, 35);
    luongInput.setPosition(pX + 120, pY + 250);

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

void HLVFormPopup::show(HLV* hlv, std::function<void()> onSuccess) {
    onSuccessCallback = onSuccess;
    errorMessage.setString("");
    
    if (hlv == nullptr) {
        // --- THEM MOI ---
        title.setString("Them HLV Moi");
        currentHLV = nullptr;
        // Reset cac truong
        hoTenInput.setString("");
        sdtInput.setString("");
        ngaySinhInput.setString(""); // Format: DD/MM/YYYY
        genderSelector.selectedGender = 0;
        genderSelector.btnNam.setColors(Config::CardLight, Config::CardDark);
        genderSelector.btnNu.setColors(Config::CardLight, Config::CardDark);
        chuyenMonInput.setString("");
        luongInput.setString("");
    } else {
        // --- SUA ---
        title.setString("Cap Nhat HLV");
        currentHLV = hlv;
        // Dien thong tin
        hoTenInput.setString(hlv->getHoTen());
        sdtInput.setString(hlv->getSDT());
        ngaySinhInput.setString(hlv->getNgaySinh()); // "DD/MM/YYYY"
        
        if (hlv->getGioiTinh() == "Nam") {
            genderSelector.btnNam.click();
        } else if (hlv->getGioiTinh() == "Nu") {
            genderSelector.btnNu.click();
        }
        
        chuyenMonInput.setString(hlv->getChuyenMon());
        luongInput.setString(std::to_string((int)hlv->getLuong()));
    }
    
    focusIndex = 0;
    updateFocus();
    BasePopup::show();
}

bool HLVFormPopup::validate(std::string& ten, std::string& sdt, std::string& gioiTinh, 
                            std::string& ngaySinh, std::string& chuyenMon, double& luong)
{
    errorMessage.setString("");
    
    // 1. Lay du lieu tu Form
    ten = hoTenInput.getString();//.toAnsiString(); //SFML 3.0
    sdt = sdtInput.getString();//.toAnsiString();
    gioiTinh = genderSelector.getString();
    ngaySinh = ngaySinhInput.getString();//.toAnsiString(); // Format: DD/MM/YYYY
    chuyenMon = chuyenMonInput.getString();//.toAnsiString();
    
    std::string errorMsg;
    
    // 2. Kiem tra de trong (theo yeu cau)
    if (ten.empty() || sdt.empty() || gioiTinh.empty() || ngaySinh.empty() || chuyenMon.empty() || luongInput.getString().empty()) {
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

void HLVFormPopup::handleSubmit() {
    std::string ten, sdt, gioiTinh, ngaySinh, chuyenMon;
    double luong;

    if (!validate(ten, sdt, gioiTinh, ngaySinh, chuyenMon, luong)) {
        return; // Dung lai neu validate loi
    }
    
    if (currentHLV == nullptr) {
        // --- THEM MOI (Goi Service) ---
        HLVService::themHLV(ten, sdt, gioiTinh, ngaySinh, chuyenMon, luong);
    } else {
        // --- SUA (Goi Service) ---
        HLVService::suaHLV(currentHLV->getID(), ten, sdt, gioiTinh, ngaySinh, chuyenMon, luong);
    }
    
    // (Kiem tra loi tu Service - Hien tai Service dang tu show loi)
    
    if (onSuccessCallback) {
        onSuccessCallback(); // Goi callback de load lai bang
    }
    
    hide(); // Dong popup
}

void HLVFormPopup::drawContent(sf::RenderTarget& target) {
    // Ve cac label
    sf::Text label(font, sf::String(), 16);
    label.setFillColor(Config::TextNormal);
    float pX = popupPanel.getPosition().x + 30;
    float pY = popupPanel.getPosition().y + 80;
    
    label.setString("Ho Ten:");
    // SỬA Ở ĐÂY: Dùng sf::Vector2f
    label.setPosition(sf::Vector2f(pX, pY + 5)); target.draw(label);
    
    label.setString("SDT:");
    // SỬA Ở ĐÂY: Dùng sf::Vector2f
    label.setPosition(sf::Vector2f(pX, pY + 55)); target.draw(label);
    
    label.setString("Ngay Sinh:");
    // SỬA Ở ĐÂY: Dùng sf::Vector2f
    label.setPosition(sf::Vector2f(pX, pY + 105)); target.draw(label);
    
    label.setString("Chuyen Mon:");
    // SỬA Ở ĐÂY: Dùng sf::Vector2f
    label.setPosition(sf::Vector2f(pX, pY + 205)); target.draw(label);
    
    label.setString("Luong Cung:");
    // SỬA Ở ĐÂY: Dùng sf::Vector2f
    label.setPosition(sf::Vector2f(pX, pY + 255)); target.draw(label);

    // Ve cac component
    hoTenInput.draw(target);
    sdtInput.draw(target);
    ngaySinhInput.draw(target);
    genderSelector.draw(target);
    chuyenMonInput.draw(target);
    luongInput.draw(target);
    
    target.draw(errorMessage);
    
    confirmButton.draw(target);
    cancelButton.draw(target);
}

void HLVFormPopup::handleKeyNavigation(sf::Keyboard::Key key) {
    int maxFocus = 8; // 0-5 inputs, 6-7 gender, 8-9 buttons
    
    // Logic dieu huong (ban co the chinh sua cho phu hop)
    if (key == sf::Keyboard::Key::Tab || key == sf::Keyboard::Key::Down) {
        focusIndex = (focusIndex + 1) % (maxFocus + 1);
        updateFocus();
    } 
    else if (key == sf::Keyboard::Key::Up) {
        focusIndex = (focusIndex - 1 + (maxFocus + 1)) % (maxFocus + 1);
        updateFocus();
    }
    else if (key == sf::Keyboard::Key::Enter) {
        if (focusIndex < 4) { // 0:Ten, 1:SDT, 2:NgaySinh, 3:ChuyenMon
            focusIndex++;
            updateFocus();
        } else if (focusIndex == 4) { // O input Luong
            handleSubmit();
        }
        // ... (them logic cho cac nut)
    }
}

void HLVFormPopup::updateFocus() {
    hoTenInput.setFocus(focusIndex == 0);
    sdtInput.setFocus(focusIndex == 1);
    ngaySinhInput.setFocus(focusIndex == 2);
    // (Them logic cho genderSelector focus)
    chuyenMonInput.setFocus(focusIndex == 3);
    luongInput.setFocus(focusIndex == 4);
    // ... (Them logic cho button focus)
}

void HLVFormPopup::handleEvent(sf::Event event, sf::Vector2i mousePos) {
    if (!isVisible) return;
    BasePopup::handleEvent(event, mousePos); // Nut [X]

    confirmButton.handleEvent(event, mousePos);
    cancelButton.handleEvent(event, mousePos);
    genderSelector.handleEvent(event, mousePos);
    
    // Xu ly input (TextEntered)
    if (event.getIf<sf::Event::TextEntered>()) {
        if (focusIndex == 0) hoTenInput.handleEvent(event);
        else if (focusIndex == 1) sdtInput.handleEvent(event);
        else if (focusIndex == 2) ngaySinhInput.handleEvent(event);
        else if (focusIndex == 3) chuyenMonInput.handleEvent(event);
        else if (focusIndex == 4) luongInput.handleEvent(event);
    }
    
    // Xu ly dieu huong
    if (auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        if (focusIndex == 0) hoTenInput.handleEvent(event);
        else if (focusIndex == 1) sdtInput.handleEvent(event);
        else if (focusIndex == 2) ngaySinhInput.handleEvent(event);
        else if (focusIndex == 3) chuyenMonInput.handleEvent(event);
        else if (focusIndex == 4) luongInput.handleEvent(event);
        
        handleKeyNavigation(keyEvent->code);
    }
}

void HLVFormPopup::update(sf::Vector2i mousePos) {
    if (!isVisible) return;
    BasePopup::update(mousePos); // Nut [X]

    hoTenInput.update(sf::Time::Zero);
    sdtInput.update(sf::Time::Zero);
    ngaySinhInput.update(sf::Time::Zero);
    chuyenMonInput.update(sf::Time::Zero);
    luongInput.update(sf::Time::Zero);
    
    genderSelector.update(mousePos);
    confirmButton.update(mousePos);
    cancelButton.update(mousePos);
}