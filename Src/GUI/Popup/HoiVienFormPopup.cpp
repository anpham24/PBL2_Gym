#include "HoiVienFormPopup.h"
#include "Services/HoiVienService.h"
#include "Utils/Validator.h"
#include <iostream>

// ========================================================================
// IMPLEMENTATION cho GenderSelector
// ========================================================================

// 1. Constructor
GenderSelector::GenderSelector(sf::Font& font) 
    : font(font), 
      selectedGender(0), 
      label(new sf::Text(font, "Gioi tinh:", 16)), 
      btnNam(), 
      btnNu()
{
    label->setFillColor(Config::TextNormal);
    
    btnNam.setup("Nam", font);
    btnNu.setup("Nu", font);
    btnNam.setSize(80, 35);
    btnNu.setSize(80, 35);
    
    btnNam.setColors(Config::CardLight, Config::CardDark);
    btnNu.setColors(Config::CardLight, Config::CardDark);
    
    btnNam.setOnClick([this](){ 
        selectedGender = 1; 
        btnNam.setColors(Config::AccentCyan, Config::AccentCyan);
        btnNu.setColors(Config::CardLight, Config::CardDark);
    });
    btnNu.setOnClick([this](){ 
        selectedGender = 2; 
        btnNu.setColors(Config::AccentCyan, Config::AccentCyan);
        btnNam.setColors(Config::CardLight, Config::CardDark);
    });
}

// 2. Destructor
GenderSelector::~GenderSelector() {
    delete label;
}

// 3. Copy Constructor
GenderSelector::GenderSelector(const GenderSelector& other)
    : font(other.font),
      selectedGender(other.selectedGender),
      btnNam(other.btnNam), 
      btnNu(other.btnNu)
{
    if (other.label != nullptr) {
        this->label = new sf::Text(*other.label);
    } else {
        this->label = nullptr;
    }
}

// 4. Copy Assignment Operator
GenderSelector& GenderSelector::operator=(const GenderSelector& other) {
    if (this == &other) return *this;

    selectedGender = other.selectedGender;
    btnNam = other.btnNam;
    btnNu = other.btnNu;

    delete label;
    if (other.label != nullptr) {
        this->label = new sf::Text(*other.label);
    } else {
        this->label = nullptr;
    }
    return *this;
}

void GenderSelector::setup(float x, float y) {
    if (label) label->setPosition(sf::Vector2f(x, y + 5));
    btnNam.setPosition(x + 120, y);
    btnNu.setPosition(x + 210, y);
}

void GenderSelector::draw(sf::RenderTarget& target) {
    if (label) target.draw(*label);
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
      errorMessage(app.getGlobalFont(), "", 14),
      hoTenInput(),
      sdtInput(),
      ngaySinhInput(),
      confirmButton(),
      cancelButton()
{
    sf::Vector2f panelSize(500, 450);
    
    // SỬA LẠI: Dùng '.' (dấu chấm) vì popupPanel là đối tượng, không phải con trỏ
    popupPanel.setSize(panelSize);
    popupPanel.setPosition(sf::Vector2f(
        (app.getWindow().getSize().x - panelSize.x) / 2.0f,
        (app.getWindow().getSize().y - panelSize.y) / 2.0f
    ));
    
    // SỬA LẠI: Dùng '.'
    title.setPosition(sf::Vector2f(popupPanel.getPosition().x + 20, popupPanel.getPosition().y + 15));
    
    // SỬA LẠI: Dùng '.'
    closeButton.setPosition(
        popupPanel.getPosition().x + panelSize.x - 40, 
        popupPanel.getPosition().y + 15
    );
    
    // SỬA LẠI: Dùng '.'
    float pX = popupPanel.getPosition().x + 30;
    float pY = popupPanel.getPosition().y + 80;
    float inputWidth = panelSize.x - 180;

    sf::Font& f = app.getGlobalFont();

    hoTenInput.setup("Nhap ho ten", f, false);
    hoTenInput.setSize(inputWidth, 35);
    hoTenInput.setPosition(pX + 120, pY);

    sdtInput.setup("Nhap SDT (VD: 09...)", f, false);
    sdtInput.setSize(inputWidth, 35);
    sdtInput.setPosition(pX + 120, pY + 50);

    ngaySinhInput.setup("DD/MM/YYYY", f, false);
    ngaySinhInput.setSize(inputWidth, 35);
    ngaySinhInput.setPosition(pX + 120, pY + 100);

    genderSelector.setup(pX, pY + 150);

    errorMessage.setFillColor(Config::Danger);
    errorMessage.setPosition(sf::Vector2f(pX, pY + 200));

    // SỬA LẠI: Dùng '.'
    float btnY = popupPanel.getPosition().y + panelSize.y - 70;
    
    confirmButton.setup("Xac Nhan", f);
    confirmButton.setSize(120, 40);
    confirmButton.setPosition(pX + 100, btnY);
    confirmButton.setOnClick([this]() {
        handleSubmit();
    });

    cancelButton.setup("Huy", f);
    cancelButton.setSize(120, 40);
    cancelButton.setPosition(pX + 260, btnY);
    cancelButton.setOnClick([this]() {
        hide();
    });
}

void HoiVienFormPopup::show(HoiVien* hv, std::function<void()> onSuccess) {
    onSuccessCallback = onSuccess;
    errorMessage.setString("");
    
    if (hv == nullptr) {
        // SỬA LẠI: Dùng '.'
        title.setString("Them Hoi Vien Moi");
        currentHoiVien = nullptr;
        
        hoTenInput.setString("");
        sdtInput.setString("");
        ngaySinhInput.setString("");
        genderSelector.selectedGender = 0;
        genderSelector.btnNam.setColors(Config::CardLight, Config::CardDark);
        genderSelector.btnNu.setColors(Config::CardLight, Config::CardDark);
    } else {
        // SỬA LẠI: Dùng '.'
        title.setString("Cap Nhat Thong Tin");
        currentHoiVien = hv;
        
        hoTenInput.setString(hv->getHoTen());
        sdtInput.setString(hv->getSDT());
        ngaySinhInput.setString(hv->getNgaySinh());
        
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
    sf::Font& f = genderSelector.font; 
    
    sf::Text label(f, "", 16);
    label.setFillColor(Config::TextNormal);
    
    // SỬA LẠI: Dùng '.'
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
    BasePopup::handleEvent(event, mousePos);

    confirmButton.handleEvent(event, mousePos);
    cancelButton.handleEvent(event, mousePos);
    genderSelector.handleEvent(event, mousePos);

    if (event.getIf<sf::Event::TextEntered>()) {
        if (focusIndex == 0) hoTenInput.handleEvent(event);
        else if (focusIndex == 1) sdtInput.handleEvent(event);
        else if (focusIndex == 2) ngaySinhInput.handleEvent(event);
    }
    
    if (auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        if (focusIndex == 0) hoTenInput.handleEvent(event);
        else if (focusIndex == 1) sdtInput.handleEvent(event);
        else if (focusIndex == 2) ngaySinhInput.handleEvent(event);

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
            case 0: 
            case 1: 
            case 2: 
                focusIndex++;
                updateFocus();
                break;
            case 3: 
                genderSelector.btnNam.click();
                break;
            case 4: 
                genderSelector.btnNu.click();
                break;
            case 5: 
                confirmButton.click();
                break;
            case 6: 
                cancelButton.click();
                break;
        }
    }
}

void HoiVienFormPopup::update(sf::Vector2i mousePos) {
    if (!isVisible) return;
    BasePopup::update(mousePos);

    hoTenInput.update(sf::Time::Zero);
    sdtInput.update(sf::Time::Zero);
    ngaySinhInput.update(sf::Time::Zero);
    
    genderSelector.update(mousePos);
    confirmButton.update(mousePos);
    cancelButton.update(mousePos);
}

bool HoiVienFormPopup::validateInfo(std::string& ten, std::string& sdt, std::string& gioiTinh, std::string& ngaySinh) {
    errorMessage.setString("");
    
    ten = hoTenInput.getString();
    sdt = sdtInput.getString();
    gioiTinh = genderSelector.getString();
    ngaySinh = ngaySinhInput.getString();

    if (ten.empty() || sdt.empty() || gioiTinh.empty() || ngaySinh.empty()) {
        errorMessage.setString("Vui long nhap du thong tin");
        return false;
    }

    std::string errorMsg;
    
    errorMsg = Validator::validateTen(ten);
    if (!errorMsg.empty()) { errorMessage.setString(errorMsg); return false; }
    
    errorMsg = Validator::validateSDT(sdt);
    if (!errorMsg.empty()) { errorMessage.setString(errorMsg); return false; }

    errorMsg = Validator::validateNgay(ngaySinh);
    if (!errorMsg.empty()) { errorMessage.setString(errorMsg); return false; }

    return true;
}

void HoiVienFormPopup::handleSubmit() {
    std::string ten, sdt, gioiTinh, ngaySinh;
    
    if (!validateInfo(ten, sdt, gioiTinh, ngaySinh)) {
        return;
    }

    if (currentHoiVien == nullptr) {
        // --- THEM MOI ---
        HoiVienService::themHoiVien(ten, sdt, gioiTinh, ngaySinh, 0);
    } else {
        // --- CAP NHAT ---
        HoiVienService::suaHoiVien(
            currentHoiVien->getID(), 
            ten, 
            sdt, 
            gioiTinh, 
            ngaySinh, 
            currentHoiVien->getPoint()
        );
    }
    
    if (onSuccessCallback) {
        onSuccessCallback();
    }
    
    hide();
}