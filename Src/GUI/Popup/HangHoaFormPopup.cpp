// GUI/Popups/HangHoaFormPopup.cpp
#include "HangHoaFormPopup.h"
#include "HangHoaService.h"
#include "Validator.h"

// ========================================================================
// IMPLEMENTATION cho LoaiHHSelector (Component con)
// ========================================================================

LoaiHHSelector::LoaiHHSelector(sf::Font& font) 
    : font(font), selectedLoai(""), label(font, "Loai hang hoa:", 16), btnThucPham(), btnDungCu(), btnKhac()
{
    // label.setFont(font);
    // label.setString("Loai hang hoa:");
    // label.setCharacterSize(16);
    label.setFillColor(Config::TextNormal);
    
    btnThucPham.setup("Thuc pham", font);
    btnDungCu.setup("Dung cu", font);
    btnKhac.setup("Khac", font);
    
    sf::Vector2f btnSize(100, 35);
    btnThucPham.setSize(btnSize.x, btnSize.y);
    btnDungCu.setSize(btnSize.x, btnSize.y);
    btnKhac.setSize(btnSize.x, btnSize.y);
    
    // Dat mau mac dinh
    btnThucPham.setColors(Config::CardLight, Config::CardDark);
    btnDungCu.setColors(Config::CardLight, Config::CardDark);
    btnKhac.setColors(Config::CardLight, Config::CardDark);
    
    // Gan callback
    btnThucPham.setOnClick([this](){ setSelected("Thuc pham"); });
    btnDungCu.setOnClick([this](){ setSelected("Dung cu"); });
    btnKhac.setOnClick([this](){ setSelected("Khac"); });
}

void LoaiHHSelector::setup(float x, float y) {
    label.setPosition(sf::Vector2f(x, y + 5));
    btnThucPham.setPosition(x + 130, y);
    btnDungCu.setPosition(x + 240, y);
    btnKhac.setPosition(x + 350, y);
}

void LoaiHHSelector::draw(sf::RenderTarget& target) {
    target.draw(label);
    btnThucPham.draw(target);
    btnDungCu.draw(target);
    btnKhac.draw(target);
}

void LoaiHHSelector::handleEvent(sf::Event event, sf::Vector2i mousePos) {
    btnThucPham.handleEvent(event, mousePos);
    btnDungCu.handleEvent(event, mousePos);
    btnKhac.handleEvent(event, mousePos);
}

void LoaiHHSelector::update(sf::Vector2i mousePos) {
    btnThucPham.update(mousePos);
    btnDungCu.update(mousePos);
    btnKhac.update(mousePos);
}

void LoaiHHSelector::setSelected(const std::string& loai) {
    selectedLoai = loai;
    
    // Reset mau
    btnThucPham.setColors(Config::CardLight, Config::CardDark);
    btnDungCu.setColors(Config::CardLight, Config::CardDark);
    btnKhac.setColors(Config::CardLight, Config::CardDark);
    
    // To mau nut duoc chon
    if (loai == "Thuc pham") {
        btnThucPham.setColors(Config::AccentCyan, Config::AccentCyan);
    } else if (loai == "Dung cu") {
        btnDungCu.setColors(Config::AccentCyan, Config::AccentCyan);
    } else if (loai == "Khac") {
        btnKhac.setColors(Config::AccentCyan, Config::AccentCyan);
    }
}

void LoaiHHSelector::setFocused(bool focus1, bool focus2, bool focus3) {
    btnThucPham.setFocused(focus1);
    btnDungCu.setFocused(focus2);
    btnKhac.setFocused(focus3);
}


// ========================================================================
// IMPLEMENTATION cho HangHoaFormPopup
// ========================================================================

HangHoaFormPopup::HangHoaFormPopup(App& app)
    : BasePopup(app, "Them Hang Hoa"), 
      currentHangHoa(nullptr),
      loaiSelector(app.getGlobalFont()),
      focusIndex(0),
      errorMessage(font, "", 14)
{
    // Thiet ke layout
    sf::Vector2f panelSize(500, 400);
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

    tenHHInput.setup("Nhap ten hang hoa", font, false);
    tenHHInput.setSize(inputWidth, 35);
    tenHHInput.setPosition(pX + 130, pY);

    loaiSelector.setup(pX, pY + 50);

    giaInput.setup("Nhap gia (VND)", font, false);
    giaInput.setSize(inputWidth, 35);
    giaInput.setPosition(pX + 130, pY + 100);

    soLuongInput.setup("Nhap so luong", font, false);
    soLuongInput.setSize(inputWidth, 35);
    soLuongInput.setPosition(pX + 130, pY + 150);

    // errorMessage.setFont(font);
    // errorMessage.setCharacterSize(14);
    errorMessage.setFillColor(Config::Danger);
    errorMessage.setPosition(sf::Vector2f(pX, pY + 200));

    // Nut bam
    float btnY = popupPanel.getPosition().y + panelSize.y - 70;
    confirmButton.setup("Xac Nhan", font);
    confirmButton.setSize(120, 40);
    confirmButton.setPosition(pX + 70, btnY);
    confirmButton.setOnClick([this]() { handleSubmit(); });

    cancelButton.setup("Huy", font);
    cancelButton.setSize(120, 40);
    cancelButton.setPosition(pX + 260, btnY);
    cancelButton.setOnClick([this]() { hide(); });
}

void HangHoaFormPopup::show(HangHoa* hh, std::function<void()> onSuccess) {
    onSuccessCallback = onSuccess;
    errorMessage.setString("");
    
    if (hh == nullptr) {
        // --- THEM MOI ---
        title.setString("Them Hang Hoa Moi");
        currentHangHoa = nullptr;
        tenHHInput.setString("");
        loaiSelector.setSelected(""); // Reset
        giaInput.setString("");
        soLuongInput.setString("");
    } else {
        // --- SUA ---
        title.setString("Cap Nhat Hang Hoa");
        currentHangHoa = hh;
        tenHHInput.setString(hh->getTenHH());
        loaiSelector.setSelected(hh->getLoaiHH());
        giaInput.setString(std::to_string((int)hh->getGia()));
        soLuongInput.setString(std::to_string(hh->getSoLuongCon()));
    }
    
    focusIndex = 0;
    updateFocus();
    BasePopup::show();
}

bool HangHoaFormPopup::validate(std::string& ten, std::string& loai, double& gia, int& soLuong)
{
    errorMessage.setString("");
    
    // 1. Lay du lieu
    ten = tenHHInput.getString();//.toAnsiString();
    loai = loaiSelector.selectedLoai;

    // 2. Kiem tra de trong
    if (ten.empty() || loai.empty() || giaInput.getString().empty() || soLuongInput.getString().empty()) {
        errorMessage.setString("Vui long nhap du thong tin");
        return false;
    }

    std::string errorMsg;
    
    // 3. Validate Ten
    errorMsg = Validator::validateTen(ten);
    if (!errorMsg.empty()) { errorMessage.setString(errorMsg); return false; }
    
    // 4. Validate Gia
    try {
        gia = std::stod(giaInput.getString());//.toAnsiString());
        errorMsg = Validator::validateSoDuong(gia);
        if (!errorMsg.empty() || gia == 0) { 
            errorMessage.setString("Gia phai la so duong"); return false; 
        }
    } catch (...) {
        errorMessage.setString("Gia phai la so"); return false;
    }

    // 5. Validate So Luong
    try {
        soLuong = std::stoi(soLuongInput.getString());//.toAnsiString());
        errorMsg = Validator::validateSoDuong(soLuong); // Kiem tra < 0
        if (!errorMsg.empty()) { 
            errorMessage.setString("So luong phai la so nguyen khong am"); return false; 
        }
    } catch (...) {
        errorMessage.setString("So luong phai la so nguyen"); return false;
    }
    
    return true;
}

void HangHoaFormPopup::handleSubmit() {
    std::string ten, loai;
    double gia;
    int soLuong;

    if (!validate(ten, loai, gia, soLuong)) {
        return; // Dung lai neu validate loi
    }
    
    if (currentHangHoa == nullptr) {
        // --- THEM MOI (Goi Service) ---
        HangHoaService::themHangHoa(ten, loai, gia, soLuong);
    } else {
        // --- SUA (Goi Service) ---
        HangHoaService::suaHangHoa(currentHangHoa->getID(), ten, loai, gia, soLuong);
    }
    
    if (onSuccessCallback) onSuccessCallback();
    hide();
}

void HangHoaFormPopup::drawContent(sf::RenderTarget& target) {
    // Ve cac label
    sf::Text label(font, "", 16);
    label.setFillColor(Config::TextNormal);
    float pX = popupPanel.getPosition().x + 30;
    float pY = popupPanel.getPosition().y + 80;
    
    label.setString("Ten Hang Hoa:");
    label.setPosition(sf::Vector2f(pX, pY + 5)); target.draw(label);
    
    label.setString("Gia (VND):");
    label.setPosition(sf::Vector2f(pX, pY + 105)); target.draw(label);
    
    label.setString("So Luong:");
    label.setPosition(sf::Vector2f(pX, pY + 155)); target.draw(label);

    // Ve inputs
    tenHHInput.draw(target);
    loaiSelector.draw(target);
    giaInput.draw(target);
    soLuongInput.draw(target);
    
    target.draw(errorMessage);
    confirmButton.draw(target);
    cancelButton.draw(target);
}

void HangHoaFormPopup::handleKeyNavigation(sf::Keyboard::Key key) {
    // 0:Ten, 1:Loai1, 2:Loai2, 3:Loai3, 4:Gia, 5:SL, 6:Confirm, 7:Cancel
    int maxFocus = 7;
    
    if (key == sf::Keyboard::Key::Tab || key == sf::Keyboard::Key::Down) {
        focusIndex = (focusIndex + 1) % (maxFocus + 1);
    } 
    else if (key == sf::Keyboard::Key::Up) {
        focusIndex = (focusIndex - 1 + (maxFocus + 1)) % (maxFocus + 1);
    }
    else if (key == sf::Keyboard::Key::Enter) {
        if (focusIndex == 0) focusIndex = 1; // Ten -> Loai 1
        else if (focusIndex == 1) loaiSelector.btnThucPham.click();
        else if (focusIndex == 2) loaiSelector.btnDungCu.click();
        else if (focusIndex == 3) loaiSelector.btnKhac.click();
        else if (focusIndex == 4) focusIndex = 5; // Gia -> So Luong
        else if (focusIndex == 5) handleSubmit(); // So Luong -> Submit
        else if (focusIndex == 6) confirmButton.click();
        else if (focusIndex == 7) cancelButton.click();
    }
    updateFocus();
}

void HangHoaFormPopup::updateFocus() {
    tenHHInput.setFocus(focusIndex == 0);
    loaiSelector.setFocused(focusIndex == 1, focusIndex == 2, focusIndex == 3);
    giaInput.setFocus(focusIndex == 4);
    soLuongInput.setFocus(focusIndex == 5);
    confirmButton.setFocused(focusIndex == 6);
    cancelButton.setFocused(focusIndex == 7);
}

void HangHoaFormPopup::handleEvent(sf::Event event, sf::Vector2i mousePos) {
    if (!isVisible) return;
    BasePopup::handleEvent(event, mousePos); // Nut [X]

    confirmButton.handleEvent(event, mousePos);
    cancelButton.handleEvent(event, mousePos);
    loaiSelector.handleEvent(event, mousePos);
    
    // Xu ly input (TextEntered)
    if (event.getIf<sf::Event::TextEntered>()) {
        if (focusIndex == 0) tenHHInput.handleEvent(event);
        else if (focusIndex == 4) giaInput.handleEvent(event);
        else if (focusIndex == 5) soLuongInput.handleEvent(event);
    }
    
    // Xu ly dieu huong
    if (auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        if (focusIndex == 0) tenHHInput.handleEvent(event);
        else if (focusIndex == 4) giaInput.handleEvent(event);
        else if (focusIndex == 5) soLuongInput.handleEvent(event);
        
        handleKeyNavigation(keyEvent->code);
    }
}

void HangHoaFormPopup::update(sf::Vector2i mousePos) {
    if (!isVisible) return;
    BasePopup::update(mousePos); // Nut [X]

    tenHHInput.update(sf::Time::Zero);
    giaInput.update(sf::Time::Zero);
    soLuongInput.update(sf::Time::Zero);
    
    loaiSelector.update(mousePos);
    confirmButton.update(mousePos);
    cancelButton.update(mousePos);
}