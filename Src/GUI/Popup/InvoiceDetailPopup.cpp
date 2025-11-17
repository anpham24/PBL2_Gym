// GUI/Popups/InvoiceDetailPopup.cpp
#include "InvoiceDetailPopup.h"
#include "HoiVien.h"
#include "NhanVien.h"
#include "HangHoa.h"
#include "GoiTap.h"
#include "ChiTietHoaDon_HH.h"
#include "ChiTietHoaDon_GT.h"
#include <string>

InvoiceDetailPopup::InvoiceDetailPopup(App& app)
    : BasePopup(app, "Chi Tiet Hoa Don"), // Tieu de
      currentHoaDon(nullptr),
      totalLine(font, "", 18)
{
    // Thiet ke layout (2 cot)
    sf::Vector2f panelSize(600, 500);
    popupPanel.setSize(panelSize);
    popupPanel.setPosition(sf::Vector2f(
        (app.getWindow().getSize().x - panelSize.x) / 2.0f,
        (app.getWindow().getSize().y - panelSize.y) / 2.0f
    ));
    // Dat lai vi tri title/nut [X]
    title.setPosition(sf::Vector2f(popupPanel.getPosition().x + 20, popupPanel.getPosition().y + 15));
    closeButton.setPosition(popupPanel.getPosition().x + panelSize.x - 40, popupPanel.getPosition().y + 15);
    
    // Thiet lap text tong tien
    // totalLine.setFont(font);
    // totalLine.setCharacterSize(18);
    totalLine.setStyle(sf::Text::Bold);
    totalLine.setFillColor(Config::Success);
}

void InvoiceDetailPopup::show(HoaDon* hd) {
    if (hd == nullptr) return;
    currentHoaDon = hd;
    title.setString("Chi Tiet Hoa Don: " + sf::String::fromUtf8(hd->getID().begin(), hd->getID().end()));
    
    loadData(); // Nap du lieu
    
    BasePopup::show();
}

void InvoiceDetailPopup::hide() {
    BasePopup::hide();
    currentHoaDon = nullptr; // Reset
    infoLines.clear();
    itemLines.clear();
}

void InvoiceDetailPopup::loadData() {
    if (currentHoaDon == nullptr) return;
    
    // Xoa du lieu cu
    infoLines.clear();
    itemLines.clear();
    
    float pX_col1 = popupPanel.getPosition().x + 30;
    float pX_col2 = popupPanel.getPosition().x + 320;
    float pY = popupPanel.getPosition().y + 80; // Vi tri bat dau noi dung

    // --- Cot 1: Thong Tin ---
    sf::Text line(font, "", 16);
    line.setFillColor(Config::TextNormal);
    
    std::string khachHang = (currentHoaDon->getHoiVien() != nullptr) ? 
                            currentHoaDon->getHoiVien()->getHoTen() : "Khach le";
    line.setString("Khach hang: " + sf::String::fromUtf8(khachHang.begin(), khachHang.end()));
    line.setPosition(sf::Vector2f(pX_col1, pY));
    infoLines.push_back(line);
    
    std::string nhanVien = (currentHoaDon->getNhanVien() != nullptr) ?
                           currentHoaDon->getNhanVien()->getHoTen() : "N/A";
    line.setString("Nhan vien: " + sf::String::fromUtf8(nhanVien.begin(), nhanVien.end()));
    line.setPosition(sf::Vector2f(pX_col1, pY + 30));
    infoLines.push_back(line);

    line.setString("Ngay lap: " + sf::String::fromUtf8(currentHoaDon->getNgayLap().begin(), currentHoaDon->getNgayLap().end()));
    line.setPosition(sf::Vector2f(pX_col1, pY + 60));
    infoLines.push_back(line);
    
    line.setString("Thanh toan: " + sf::String::fromUtf8(currentHoaDon->getPhuongThuc().begin(), currentHoaDon->getPhuongThuc().end()));
    line.setPosition(sf::Vector2f(pX_col1, pY + 90));
    infoLines.push_back(line);

    // --- Cot 2: Chi Tiet Item ---
    float itemY = pY;
    int stt = 1;
    
    sf::Text itemText(font, "", 14);
    itemText.setFillColor(Config::TextNormal);
    sf::Text priceText(font, "", 14);
    priceText.setFillColor(Config::TextMuted);

    // Ve Hang Hoa
    const auto& dsHH = currentHoaDon->getDsChiTietHoaDon_HH();
    for(size_t i = 0; i < dsHH.size(); ++i) {
        ChiTietHoaDon_HH* ct = dsHH[i];
        std::string s = std::to_string(stt++) + ". " + ct->getHangHoa()->getTenHH();
        itemText.setString(sf::String::fromUtf8(s.begin(), s.end()));
        itemText.setPosition(sf::Vector2f(pX_col2, itemY));
        itemLines.push_back(itemText);
        
        s = "   (" + std::to_string(ct->getSoLuong()) + " x " + std::to_string((int)ct->getDonGia()) + ")";
        priceText.setString(sf::String::fromUtf8(s.begin(), s.end()));
        priceText.setPosition(sf::Vector2f(pX_col2, itemY + 18));
        itemLines.push_back(priceText);
        
        itemY += 40;
    }
    
    // Ve Goi Tap
    const auto& dsGT = currentHoaDon->getDsChiTietHoaDon_GT();
    for(size_t i = 0; i < dsGT.size(); ++i) {
        ChiTietHoaDon_GT* ct = dsGT[i];
        std::string s = std::to_string(stt++) + ". " + ct->getGoiTap()->getTenGoi();
        itemText.setString(sf::String::fromUtf8(s.begin(), s.end()));
        itemText.setPosition(sf::Vector2f(pX_col2, itemY));
        itemLines.push_back(itemText);

        s = "   (" + std::to_string(ct->getSoLuong()) + " x " + std::to_string((int)ct->getDonGia()) + ")";
        priceText.setString(sf::String::fromUtf8(s.begin(), s.end()));
        priceText.setPosition(sf::Vector2f(pX_col2, itemY + 18));
        itemLines.push_back(priceText);
        
        itemY += 40;
    }
    
    // Tong tien
    itemY = popupPanel.getPosition().y + popupPanel.getSize().y - 100; // Dat o gan cuoi
    totalLine.setPosition(sf::Vector2f(pX_col2, itemY));
    totalLine.setString("Tong Thanh Toan: " + std::to_string((int)currentHoaDon->getTotal()) + " VND");
}

void InvoiceDetailPopup::drawContent(sf::RenderTarget& target) {
    // Ve cot 1
    for (const auto& line : infoLines) {
        target.draw(line);
    }
    // Ve cot 2
    for (const auto& line : itemLines) {
        target.draw(line);
    }
    // Ve tong tien
    target.draw(totalLine);
}

// --- Xu ly Event & Update ---
void InvoiceDetailPopup::handleEvent(sf::Event event, sf::Vector2i mousePos) {
    if (!isVisible) return;
    BasePopup::handleEvent(event, mousePos); // Chi xu ly nut [X]
}

void InvoiceDetailPopup::update(sf::Vector2i mousePos) {
    if (!isVisible) return;
    BasePopup::update(mousePos); // Chi cap nhat nut [X]
}