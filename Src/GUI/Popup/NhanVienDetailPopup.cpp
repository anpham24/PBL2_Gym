// GUI/Popups/NhanVienDetailPopup.cpp
#include "NhanVienDetailPopup.h"
#include "Config.h" // De lay Config::COMMISSION_RATE_STAFF
#include "Models/HoaDon.h" // De tinh doanh thu
#include <string>

NhanVienDetailPopup::NhanVienDetailPopup(App& app)
    : BasePopup(app, "Chi Tiet Nhan Vien"), // Tieu de
      currentNhanVien(nullptr)
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
}

void NhanVienDetailPopup::show(NhanVien* nv) {
    if (nv == nullptr) return;
    currentNhanVien = nv;
    title.setString("Chi Tiet: " + sf::String::fromUtf8(nv->getHoTen().begin(), nv->getHoTen().end()));
    
    loadData(); // Nap du lieu
    
    BasePopup::show();
}

void NhanVienDetailPopup::hide() {
    BasePopup::hide();
    currentNhanVien = nullptr; // Reset
    infoLines.clear();
    luongLines.clear();
}

void NhanVienDetailPopup::loadData() {
    if (currentNhanVien == nullptr) return;
    
    // Xoa du lieu cu
    infoLines.clear();
    luongLines.clear();
    
    float pY = popupPanel.getPosition().y + 80; // Vi tri bat dau noi dung (duoi title)

    // --- Load Thong Tin Ca Nhan ---
    createInfoLine(infoLines, "ID", currentNhanVien->getID(), pY);
    createInfoLine(infoLines, "Ho Ten", currentNhanVien->getHoTen(), pY + 30);
    createInfoLine(infoLines, "So Dien Thoai", currentNhanVien->getSDT(), pY + 60);
    createInfoLine(infoLines, "Ngay Sinh", currentNhanVien->getNgaySinh(), pY + 90);
    createInfoLine(infoLines, "Gioi Tinh", currentNhanVien->getGioiTinh(), pY + 120);
    createInfoLine(infoLines, "Trang Thai", currentNhanVien->getIsActive() ? "Dang lam" : "Nghi viec", pY + 150);

    // --- Load Chi Tiet Luong (Theo yeu cau cua ban) ---
    float luongY = pY + 200; // Vi tri bat dau cho luong (duoi thong tin)
    
    double luongCung = currentNhanVien->getLuong();
    
    // Tinh tong doanh thu tu Hoa Don
    double tongDoanhThu = 0;
    const MyVector<HoaDon*>& dsHoaDon = currentNhanVien->getDsHoaDon();
    for(size_t i = 0; i < dsHoaDon.size(); ++i) {
        if (dsHoaDon[i] != nullptr) {
            tongDoanhThu += dsHoaDon[i]->getTotal();
        }
    }
    
    double hoaHong = tongDoanhThu * Config::COMMISSION_RATE_STAFF;
    double tongLuong = luongCung + hoaHong;

    // Hien thi
    createInfoLine(luongLines, "Luong Cung", std::to_string((int)luongCung), luongY);
    createInfoLine(luongLines, "Tong Doanh Thu", std::to_string((int)tongDoanhThu), luongY + 30);
    
    // Hien thi chi tiet tinh hoa hong
    std::string hoaHongDetail = "( " + std::to_string((int)(Config::COMMISSION_RATE_STAFF * 100)) 
                              + "% * " + std::to_string((int)tongDoanhThu) + " ) = " 
                              + std::to_string((int)hoaHong);
    createInfoLine(luongLines, "Hoa Hong", hoaHongDetail, luongY + 60);
    
    // --- Tao dong TONG LUONG (dac biet) ---
    float labelX = popupPanel.getPosition().x + 50;
    float valueX = popupPanel.getPosition().x + 220;
    
    sf::Text label(font, "TONG LUONG:", 18);
    label.setFillColor(Config::Success);
    label.setStyle(sf::Text::Bold);
    label.setPosition(sf::Vector2f(labelX, luongY + 100));
    luongLines.push_back(label);
    
    sf::Text value(font, std::to_string((int)tongLuong) + " VND", 18);
    value.setFillColor(Config::Success);
    value.setStyle(sf::Text::Bold);
    value.setPosition(sf::Vector2f(valueX, luongY + 100));
    luongLines.push_back(value);
}

void NhanVienDetailPopup::drawContent(sf::RenderTarget& target) {
    // Ve tat ca cac dong
    for (const auto& line : infoLines) {
        target.draw(line);
    }
    for (const auto& line : luongLines) {
        target.draw(line);
    }
}

// --- Xu ly Event & Update ---
void NhanVienDetailPopup::handleEvent(sf::Event event, sf::Vector2i mousePos) {
    if (!isVisible) return;
    BasePopup::handleEvent(event, mousePos); // Chi xu ly nut [X]
}

void NhanVienDetailPopup::update(sf::Vector2i mousePos) {
    if (!isVisible) return;
    BasePopup::update(mousePos); // Chi cap nhat nut [X]
}

/**
 * @brief Ham Helper: Tao va them 2 sf::Text (label va value) vao vector
 */
void NhanVienDetailPopup::createInfoLine(std::vector<sf::Text>& lines, const std::string& label, const std::string& value, float yPos) {
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