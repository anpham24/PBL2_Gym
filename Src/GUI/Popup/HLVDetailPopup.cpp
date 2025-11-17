// GUI/Popups/HLVDetailPopup.cpp
#include "HLVDetailPopup.h"
#include "Config.h" // De lay Config::PayRateHlv...
#include "LopHoc.h" // De dem so lop
#include "LogTapPT.h" // De dem so buoi PT

HLVDetailPopup::HLVDetailPopup(App& app)
    : BasePopup(app, "Chi Tiet HLV"), // Tieu de
      tabManager(app.getGlobalFont()),
      currentHLV(nullptr)
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

    // --- Setup TabManager ---
    tabManager.setPosition(popupPanel.getPosition().x + 30, popupPanel.getPosition().y + 70);
    tabManager.addTab("Thong Tin Ca Nhan");
    tabManager.addTab("Chi Tiet Luong");
}

void HLVDetailPopup::show(HLV* hlv) {
    if (hlv == nullptr) return;
    currentHLV = hlv;
    title.setString("Chi Tiet: " + sf::String::fromUtf8(hlv->getHoTen().begin(), hlv->getHoTen().end()));
    
    loadData(); // Nap du lieu vao cac component
    
    BasePopup::show();
}

void HLVDetailPopup::hide() {
    BasePopup::hide();
    currentHLV = nullptr; // Reset
    infoLines.clear();
    luongLines.clear();
}

void HLVDetailPopup::loadData() {
    if (currentHLV == nullptr) return;
    
    // Xoa du lieu cu
    infoLines.clear();
    luongLines.clear();
    
    float pY = popupPanel.getPosition().y + 120; // Vi tri bat dau noi dung (duoi tabs)

    // --- Load Tab 1: Thong Tin Ca Nhan ---
    createInfoLine(infoLines, "ID", currentHLV->getID(), pY);
    createInfoLine(infoLines, "Ho Ten", currentHLV->getHoTen(), pY + 30);
    createInfoLine(infoLines, "So Dien Thoai", currentHLV->getSDT(), pY + 60);
    createInfoLine(infoLines, "Ngay Sinh", currentHLV->getNgaySinh(), pY + 90);
    createInfoLine(infoLines, "Gioi Tinh", currentHLV->getGioiTinh(), pY + 120);
    createInfoLine(infoLines, "Trang Thai", currentHLV->getIsActive() ? "Dang lam" : "Nghi viec", pY + 150);
    createInfoLine(infoLines, "Chuyen Mon", currentHLV->getChuyenMon(), pY + 180);
    createInfoLine(infoLines, "Luong Cung", std::to_string((int)currentHLV->getLuong()), pY + 210);

    // --- Load Tab 2: Tinh Luong (Theo yeu cau cua ban) ---
    double luongCung = currentHLV->getLuong();
    int soLopDay = currentHLV->getDsLopHoc().size();
    int soPTDay = currentHLV->getDsLogTapPT().size();
    
    double luongLop = soLopDay * Config::PAY_RATE_HLV_PER_CLASS;
    double luongPT = soPTDay * Config::PAY_RATE_HLV_PER_PT;
    double tongLuong = luongCung + luongLop + luongPT;

    createInfoLine(luongLines, "Luong Cung", std::to_string((int)luongCung), pY);
    createInfoLine(luongLines, "So Lop Da Day", std::to_string(soLopDay), pY + 30);
    createInfoLine(luongLines, "Luong Lop", 
        "( " + std::to_string(soLopDay) + " * " + std::to_string((int)Config::PAY_RATE_HLV_PER_CLASS) + " ) = " + std::to_string((int)luongLop), 
        pY + 60);
    createInfoLine(luongLines, "So Buoi PT Da Day", std::to_string(soPTDay), pY + 90);
    createInfoLine(luongLines, "Luong PT", 
        "( " + std::to_string(soPTDay) + " * " + std::to_string((int)Config::PAY_RATE_HLV_PER_PT) + " ) = " + std::to_string((int)luongPT), 
        pY + 120);
    
    // Tao dong TONG LUONG (dac biet)
    float labelX = popupPanel.getPosition().x + 50;
    float valueX = popupPanel.getPosition().x + 220;
    
    sf::Text label(font, "TONG LUONG:", 18);
    label.setFillColor(Config::Success);
    label.setStyle(sf::Text::Bold);
    label.setPosition(sf::Vector2f(labelX, pY + 170));
    luongLines.push_back(label);
    
    sf::Text value(font, std::to_string((int)tongLuong) + " VND", 18);
    value.setFillColor(Config::Success);
    value.setStyle(sf::Text::Bold);
    value.setPosition(sf::Vector2f(valueX, pY + 170));
    luongLines.push_back(value);
}

// --- Ve noi dung theo Tab ---
void HLVDetailPopup::drawContent(sf::RenderTarget& target) {
    tabManager.draw(target); // Ve thanh Tab

    if (tabManager.getActiveTab() == 0) {
        drawTabInfo(target);
    } else {
        drawTabLuong(target);
    }
}

void HLVDetailPopup::drawTabInfo(sf::RenderTarget& target) {
    // Ve cac dong thong tin da tao trong loadData()
    for (const auto& line : infoLines) {
        target.draw(line);
    }
}

void HLVDetailPopup::drawTabLuong(sf::RenderTarget& target) {
    // Ve cac dong tinh luong da tao trong loadData()
    for (const auto& line : luongLines) {
        target.draw(line);
    }
}

// --- Xu ly Event & Update ---
void HLVDetailPopup::handleEvent(sf::Event event, sf::Vector2i mousePos) {
    if (!isVisible) return;
    BasePopup::handleEvent(event, mousePos); // Xu ly nut [X]
    tabManager.handleEvent(event, mousePos); // Xu ly click tab
    
    // Khong can xu ly gi them vi day la popup chi doc
}

void HLVDetailPopup::update(sf::Vector2i mousePos) {
    if (!isVisible) return;
    BasePopup::update(mousePos); // Cap nhat nut [X]
    
    // Khong can update gi them
}

/**
 * @brief Ham Helper: Tao va them 2 sf::Text (label va value) vao vector
 */
void HLVDetailPopup::createInfoLine(std::vector<sf::Text>& lines, const std::string& label, const std::string& value, float yPos) {
    float labelX = popupPanel.getPosition().x + 50;
    float valueX = popupPanel.getPosition().x + 220;

    // Tao Label (mau mo)
    sf::Text labelText(font, sf::String::fromUtf8(label.begin(), label.end()) + ":", 16);
    labelText.setFillColor(Config::TextMuted);
    labelText.setPosition(sf::Vector2f(labelX, yPos));
    lines.push_back(labelText);

    // Tao Value (mau sang)
    sf::Text valueText(font, sf::String::fromUtf8(value.begin(), value.end()), 16);
    valueText.setFillColor(Config::TextNormal);
    valueText.setStyle(sf::Text::Bold);
    valueText.setPosition(sf::Vector2f(valueX, yPos));
    lines.push_back(valueText);
}