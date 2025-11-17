// GUI/Popups/LopHocFormPopup.cpp
#include "LopHocFormPopup.h"
#include "LopHocService.h" // Goi Service
#include "Utils/Validator.h"
#include "IDGenerator.h"
#include <iostream>

LopHocFormPopup::LopHocFormPopup(App& app)
    : BasePopup(app, "Them Lop Hoc"), 
      currentLopHoc(nullptr),
      monTapSelector(app.getGlobalFont(), "Chon mon tap..."),
      hlvSelector(app.getGlobalFont(), "Chon HLV..."),
      errorMessage(font, "", 14)  
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
    
    float pX = popupPanel.getPosition().x + 30;
    float pY = popupPanel.getPosition().y + 80;
    float inputWidth = panelSize.x - 180;

    tenLopInput.setup("Nhap ten lop", font, false);
    tenLopInput.setSize(inputWidth, 35);
    tenLopInput.setPosition(pX + 120, pY);

    lichTapInput.setup("VD: T4-18:00", font, false);
    lichTapInput.setSize(inputWidth, 35);
    lichTapInput.setPosition(pX + 120, pY + 50);

    thoiLuongInput.setup("Nhap thoi luong (phut)", font, false);
    thoiLuongInput.setSize(inputWidth, 35);
    thoiLuongInput.setPosition(pX + 120, pY + 100);

    monTapSelector.setSize(inputWidth, 35);
    monTapSelector.setPosition(pX + 120, pY + 150);
    
    hlvSelector.setSize(inputWidth, 35);
    hlvSelector.setPosition(pX + 120, pY + 200);

    // errorMessage.setFont(font);
    // errorMessage.setCharacterSize(14);
    errorMessage.setFillColor(Config::Danger);
    errorMessage.setPosition(sf::Vector2f(pX, pY + 250));

    // Nut bam
    float btnY = popupPanel.getPosition().y + panelSize.y - 70;
    confirmButton.setup("Xac Nhan", font);
    confirmButton.setSize(120, 40);
    confirmButton.setPosition(pX + 100, btnY);
    confirmButton.setOnClick([this]() { handleSubmit(); });

    cancelButton.setup("Huy", font);
    cancelButton.setSize(120, 40);
    cancelButton.setPosition(pX + 260, btnY);
    cancelButton.setOnClick([this]() { hide(); });
    
    
    // --- (QUAN TRONG) LOGIC PHU THUOC ---
    
    // Khi chon 1 Mon Tap...
    monTapSelector.setOnSelect([this](MonTap* selectedMon) {

        hlvSelector.filterItems([](HLV*){ return true; });
 });


     hlvSelector.setOnSelect([this](HLV* selectedHLV) {

        monTapSelector.filterItems([](MonTap*){ return true; });
    });
}

void LopHocFormPopup::loadSelectors() {
    // Load tat ca Mon Tap vao Selector
    monTapSelector.setItems(
        app.getQuanLy().getDsMonTap(),
        [](MonTap* mt){ return mt->getTenMon(); }
    );
    
    // Load tat ca HLV vao Selector
    hlvSelector.setItems(
        app.getQuanLy().getDsHLV(),
        [](HLV* hlv){ return hlv->getHoTen(); }
    );
}

void LopHocFormPopup::show(LopHoc* lh, std::function<void()> onSuccess) {
    onSuccessCallback = onSuccess;
    errorMessage.setString("");
    loadSelectors(); // Load data cho 2 selector
    
    if (lh == nullptr) {
        // THEM MOI
        title.setString("Them Lop Hoc Moi");
        currentLopHoc = nullptr;
        tenLopInput.setString("");
        lichTapInput.setString("");
        thoiLuongInput.setString("");
        monTapSelector.clear();
        hlvSelector.clear();
    } else {
        // SUA
        title.setString("Cap Nhat Lop Hoc");
        currentLopHoc = lh;
        tenLopInput.setString(lh->getTenLop());
        lichTapInput.setString(lh->getLichTap());
        thoiLuongInput.setString(std::to_string(lh->getThoiLuong()));
        monTapSelector.setSelected(lh->getMonTap());
        hlvSelector.setSelected(lh->getHLV());
    }
    
    BasePopup::show();
}

bool LopHocFormPopup::validate(std::string& ten, std::string& lich, int& thoiLuong, MonTap*& mt, HLV*& hlv) {
    errorMessage.setString("");
    
    ten = tenLopInput.getString();//.toAnsiString();
    lich = lichTapInput.getString();//.toAnsiString();
    mt = monTapSelector.getSelected();
    hlv = hlvSelector.getSelected();

    if (ten.empty() || lich.empty() || thoiLuongInput.getString().empty() || mt == nullptr || hlv == nullptr) {
        errorMessage.setString("Vui long dien day du thong tin");
        return false;
    }
    
    std::string errorMsg;
    
    // Validate Ten
    errorMsg = Validator::validateTen(ten);
    if (!errorMsg.empty()) { errorMessage.setString(errorMsg); return false; }
    
    // Validate Thoi Luong
    try {
        thoiLuong = std::stoi(thoiLuongInput.getString());//.toAnsiString());
        errorMsg = Validator::validateSoDuong(thoiLuong);
        if (!errorMsg.empty() || thoiLuong == 0) { 
            errorMessage.setString("Thoi luong phai la so nguyen duong"); return false; 
        }
    } catch (...) {
        errorMessage.setString("Thoi luong phai la so"); return false;
    }
    
    return true;
}

void LopHocFormPopup::handleSubmit() {
    std::string tenLop, lichTap;
    int thoiLuong;
    MonTap* mt;
    HLV* hlv;

    if (!validate(tenLop, lichTap, thoiLuong, mt, hlv)) {
        return;
    }
    
    if (currentLopHoc == nullptr) {
        // --- THEM MOI (Goi Service) ---
        LopHocService::themLopHoc(tenLop, lichTap, thoiLuong, mt->getID(), hlv->getID());
    } else {
        // --- SUA (Goi Service) ---
        LopHocService::suaLopHoc(currentLopHoc->getID(), tenLop, lichTap, thoiLuong, mt->getID(), hlv->getID());
    }
    
    if (onSuccessCallback) onSuccessCallback();
    hide();
}

void LopHocFormPopup::drawContent(sf::RenderTarget& target) {
    // Ve labels
    sf::Text label(font, "", 16);
    label.setFillColor(Config::TextNormal);
    float pX = popupPanel.getPosition().x + 30;
    float pY = popupPanel.getPosition().y + 80;
    
    label.setString("Ten Lop:");
    label.setPosition(sf::Vector2f(pX, pY + 5)); target.draw(label);
    label.setString("Lich Tap:");
    label.setPosition(sf::Vector2f(pX, pY + 55)); target.draw(label);
    label.setString("Thoi Luong:");
    label.setPosition(sf::Vector2f(pX, pY + 105)); target.draw(label);
    label.setString("Mon Tap:");
    label.setPosition(sf::Vector2f(pX, pY + 155)); target.draw(label);
    label.setString("HLV:");
    label.setPosition(sf::Vector2f(pX, pY + 205)); target.draw(label);

    // Ve inputs
    tenLopInput.draw(target);
    lichTapInput.draw(target);
    thoiLuongInput.draw(target);
    
    target.draw(errorMessage);
    confirmButton.draw(target);
    cancelButton.draw(target);

    // Ve Selector (phai ve sau cung de dropdown de len tren)
    monTapSelector.draw(target);
    hlvSelector.draw(target);
}

void LopHocFormPopup::handleEvent(sf::Event event, sf::Vector2i mousePos) {
    if (!isVisible) return;
    BasePopup::handleEvent(event, mousePos);
    
    // Truyen event cho Selector TRUOC
    // Vi chung co the "an" click de mo/dong dropdown
    monTapSelector.handleEvent(event, mousePos);
    hlvSelector.handleEvent(event, mousePos);

    // Neu dropdown dang mo, khong xu ly cac nut ben duoi
    if (monTapSelector.getIsOpen() || hlvSelector.getIsOpen()) {
        return;
    }

    // Xu ly cac nut
    confirmButton.handleEvent(event, mousePos);
    cancelButton.handleEvent(event, mousePos);
    
    // Xu ly input (chi khi khong co dropdown nao mo)
    if (event.getIf<sf::Event::TextEntered>()) {
         tenLopInput.handleEvent(event);
         lichTapInput.handleEvent(event);
         thoiLuongInput.handleEvent(event);
    }
    
    // (Bo qua dieu huong phim cho form phuc tap nay)
}

void LopHocFormPopup::update(sf::Vector2i mousePos) {
    if (!isVisible) return;
    BasePopup::update(mousePos);
    
    confirmButton.update(mousePos);
    cancelButton.update(mousePos);
    
    tenLopInput.update(sf::Time::Zero);
    lichTapInput.update(sf::Time::Zero);
    thoiLuongInput.update(sf::Time::Zero);

    // Luon update Selector (de chung co the ve dropdown)
    monTapSelector.update(mousePos);
    hlvSelector.update(mousePos);
}