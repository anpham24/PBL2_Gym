// GUI/Popups/GoiTapFormPopup.cpp
#include "GoiTapFormPopup.h"
#include "GoiTapService.h"
#include "Validator.h"
#include "IDGenerator.h"

/*
 * (Implementation cho ThoiGianSelector da duoc XOA)
 */

// ========================================================================
// IMPLEMENTATION cho GoiTapFormPopup
// ========================================================================

GoiTapFormPopup::GoiTapFormPopup(App& app)
    : BasePopup(app, "Them Goi Tap"), 
      currentGoiTap(nullptr),
      focusIndex(0),
      errorMessage(font, "", 14)
{
    // Thiet ke layout
    sf::Vector2f panelSize(550, 450);
    popupPanel.setSize(panelSize);
    popupPanel.setPosition(sf::Vector2f(
        (app.getWindow().getSize().x - panelSize.x) / 2.0f,
        (app.getWindow().getSize().y - panelSize.y) / 2.0f
    ));
    // Dat lai vi tri title/nut [X]
    title.setPosition(sf::Vector2f(popupPanel.getPosition().x + 20, popupPanel.getPosition().y + 15));
    closeButton.setPosition(popupPanel.getPosition().x + panelSize.x - 40, popupPanel.getPosition().y + 15);
    
    float pX = popupPanel.getPosition().x + 30; // X bat dau
    float pY = popupPanel.getPosition().y + 80; // Y bat dau
    float inputWidth = panelSize.x - 200; // Chieu rong input

    tenGoiInput.setup("Nhap ten goi tap", font, false);
    tenGoiInput.setSize(inputWidth, 35);
    tenGoiInput.setPosition(pX + 140, pY);

    // (MOI) Thay the ThoiGianSelector bang InputBox
    thoiGianInput.setup("Nhap so ngay (VD: 30, 90)", font, false);
    thoiGianInput.setSize(inputWidth, 35);
    thoiGianInput.setPosition(pX + 140, pY + 50);

    giaInput.setup("Nhap gia (VND)", font, false);
    giaInput.setSize(inputWidth, 35);
    giaInput.setPosition(pX + 140, pY + 100);

    soBuoiPTInput.setup("Nhap so buoi tap voi HLV (VD: 0, 5...)", font, false);
    soBuoiPTInput.setSize(inputWidth, 35);
    soBuoiPTInput.setPosition(pX + 140, pY + 150);

    // errorMessage.setFont(font);
    // errorMessage.setCharacterSize(14);
    errorMessage.setFillColor(Config::Danger);
    errorMessage.setPosition(sf::Vector2f(pX, pY + 200));

    // Nut bam
    float btnY = popupPanel.getPosition().y + panelSize.y - 70;
    confirmButton.setup("Xac Nhan", font);
    confirmButton.setSize(120, 40);
    confirmButton.setPosition(pX + 100, btnY);
    confirmButton.setOnClick([this]() { handleSubmit(); });

    cancelButton.setup("Huy", font);
    cancelButton.setSize(120, 40);
    cancelButton.setPosition(pX + 300, btnY);
    cancelButton.setOnClick([this]() { hide(); });
}

void GoiTapFormPopup::show(GoiTap* gt, std::function<void()> onSuccess) {
    onSuccessCallback = onSuccess;
    errorMessage.setString("");
    focusIndex = 0;
    
    if (gt == nullptr) {
        // THEM MOI
        title.setString("Them Goi Tap Moi");
        currentGoiTap = nullptr;
        tenGoiInput.setString("");
        thoiGianInput.setString("");
        giaInput.setString("");
        soBuoiPTInput.setString("");
    } else {
        // SUA
        title.setString("Cap Nhat Goi Tap");
        currentGoiTap = gt;
        tenGoiInput.setString(gt->getTenGoi());
        thoiGianInput.setString(std::to_string(gt->getThoiGian()));
        giaInput.setString(std::to_string((int)gt->getGia()));
        soBuoiPTInput.setString(std::to_string(gt->getSoBuoiPT()));
    }
    updateFocus();
    BasePopup::show();
}

bool GoiTapFormPopup::validate(std::string& ten, int& thoiGian, double& gia, int& soBuoiPT) {
    errorMessage.setString("");
    ten = tenGoiInput.getString();
    
    std::string errorMsg;
    
    // 1. Kiem tra Ten (Goi Validator)
    errorMsg = Validator::validateTen(ten);
    if (!errorMsg.empty()) { errorMessage.setString(errorMsg); return false; }
    
    // 2. Kiem tra Thoi Gian (So ngay)
    try {
        thoiGian = std::stoi(thoiGianInput.getString());
        errorMsg = Validator::validateSoDuong(thoiGian);
        if (!errorMsg.empty() || thoiGian == 0) { 
            errorMessage.setString("Thoi gian (so ngay) phai la so nguyen duong"); return false; 
        }
    } catch (...) {
        errorMessage.setString("Thoi gian (so ngay) phai la so"); return false;
    }

    // 3. Kiem tra Gia
    try {
        gia = std::stod(giaInput.getString());
        errorMsg = Validator::validateSoDuong(gia);
        if (!errorMsg.empty() || gia == 0) { 
            errorMessage.setString("Gia phai la so nguyen duong"); return false; 
        }
    } catch (...) {
        errorMessage.setString("Gia phai la so"); return false;
    }
    
    // 4. Kiem tra So Buoi PT
    try {
        soBuoiPT = std::stoi(soBuoiPTInput.getString());
        errorMsg = Validator::validateSoDuong(soBuoiPT); // (validateSoDuong chi kiem tra < 0)
        if (!errorMsg.empty()) { 
            errorMessage.setString("So buoi PT phai la so nguyen >= 0"); return false; 
        }
        if (soBuoiPT > thoiGian) {
             errorMessage.setString("So buoi PT khong duoc vuot qua so ngay trong goi");
            return false;
        }
    } catch (...) {
        errorMessage.setString("So buoi PT phai la so nguyen"); return false;
    }
    
    return true;
}

void GoiTapFormPopup::handleSubmit() {
    std::string tenGoi;
    int thoiGian;
    double gia;
    int soBuoiPT;

    if (!validate(tenGoi, thoiGian, gia, soBuoiPT)) {
        return; // Dung lai neu validate loi
    }
    
    if (currentGoiTap == nullptr) {
        // --- THEM MOI (Goi Service) ---
        GoiTapService::themGoiTap(tenGoi, thoiGian, soBuoiPT, gia);
    } else {
        // --- SUA (Goi Service) ---
        GoiTapService::suaGoiTap(currentGoiTap->getID(), tenGoi, thoiGian, soBuoiPT, gia);
    }
    
    if (onSuccessCallback) onSuccessCallback();
    hide();
}

void GoiTapFormPopup::drawContent(sf::RenderTarget& target) {
    // Ve cac label
    sf::Text label(font, "", 16);
    label.setFillColor(Config::TextNormal);
    float pX = popupPanel.getPosition().x + 30;
    float pY = popupPanel.getPosition().y + 80;
    
    label.setString("Ten Goi Tap:");
    label.setPosition(sf::Vector2f(pX, pY + 5)); target.draw(label);
    
    label.setString("Thoi Gian (ngay):");
    label.setPosition(sf::Vector2f(pX, pY + 55)); target.draw(label);
    
    label.setString("Gia (VND):");
    label.setPosition(sf::Vector2f(pX, pY + 105)); target.draw(label);
    
    label.setString("So buoi PT:");
    label.setPosition(sf::Vector2f(pX, pY + 155)); target.draw(label);

    // Ve inputs
    tenGoiInput.draw(target);
    thoiGianInput.draw(target);
    giaInput.draw(target);
    soBuoiPTInput.draw(target);
    
    target.draw(errorMessage);
    confirmButton.draw(target);
    cancelButton.draw(target);
}

void GoiTapFormPopup::handleKeyNavigation(sf::Keyboard::Key key) {
    int maxFocus = 5; // 0:Ten, 1:ThoiGian, 2:Gia, 3:BuoiPT, 4:Confirm, 5:Cancel
    if (key == sf::Keyboard::Key::Tab || key == sf::Keyboard::Key::Down) {
        focusIndex = (focusIndex + 1) % (maxFocus + 1);
        updateFocus();
    } 
    else if (key == sf::Keyboard::Key::Up) {
        focusIndex = (focusIndex - 1 + (maxFocus + 1)) % (maxFocus + 1);
        updateFocus();
    }
    else if (key == sf::Keyboard::Key::Enter) {
        if (focusIndex < 3) { // Neu o 3 input dau
            focusIndex++;
            updateFocus();
        } else if (focusIndex == 3) { // O input cuoi
            handleSubmit();
        } else if (focusIndex == 4) { // O nut Confirm
            confirmButton.click();
        } else if (focusIndex == 5) { // O nut Cancel
            cancelButton.click();
        }
    }
}

void GoiTapFormPopup::updateFocus() {
    tenGoiInput.setFocus(focusIndex == 0);
    thoiGianInput.setFocus(focusIndex == 1);
    giaInput.setFocus(focusIndex == 2);
    soBuoiPTInput.setFocus(focusIndex == 3);
    confirmButton.setFocused(focusIndex == 4);
    cancelButton.setFocused(focusIndex == 5);
}

void GoiTapFormPopup::handleEvent(sf::Event event, sf::Vector2i mousePos) {
    if (!isVisible) return;
    BasePopup::handleEvent(event, mousePos);
    
    confirmButton.handleEvent(event, mousePos);
    cancelButton.handleEvent(event, mousePos);
    
    // Gui TextEntered
    if (event.getIf<sf::Event::TextEntered>()) {
        if (focusIndex == 0) tenGoiInput.handleEvent(event);
        else if (focusIndex == 1) thoiGianInput.handleEvent(event);
        else if (focusIndex == 2) giaInput.handleEvent(event);
        else if (focusIndex == 3) soBuoiPTInput.handleEvent(event);
    }
    
    // Gui KeyPressed
    if (auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        if (focusIndex == 0) tenGoiInput.handleEvent(event);
        else if (focusIndex == 1) thoiGianInput.handleEvent(event);
        else if (focusIndex == 2) giaInput.handleEvent(event);
        else if (focusIndex == 3) soBuoiPTInput.handleEvent(event);
        
        handleKeyNavigation(keyEvent->code);
    }
}

void GoiTapFormPopup::update(sf::Vector2i mousePos) {
    if (!isVisible) return;
    BasePopup::update(mousePos);
    
    confirmButton.update(mousePos);
    cancelButton.update(mousePos);
    tenGoiInput.update(sf::Time::Zero);
    thoiGianInput.update(sf::Time::Zero);
    giaInput.update(sf::Time::Zero);
    soBuoiPTInput.update(sf::Time::Zero);
}