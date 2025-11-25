// GUI/Popups/HoiVienDetailPopup.cpp
#include "HoiVienDetailPopup.h"
#include <iostream>

HoiVienDetailPopup::HoiVienDetailPopup(App& app, CartPopup& cart, DatHLVPopup& hlv)
    : BasePopup(app, "Chi Tiet Hoi Vien"),
      font(app.getGlobalFont()),
      tabManager(app.getGlobalFont()),
      currentHoiVien(nullptr),
      btnDangKyGoiMoi(),
      cartPopup(cart),      // ✅ Khởi tạo reference
      datHLVPopup(hlv)      // ✅ Khởi tạo reference
{
    // Layout giữ nguyên
    sf::Vector2f panelSize(700, 500);
    popupPanel.setSize(panelSize);
    
    sf::Vector2u windowSize = app.getWindow().getSize();
    popupPanel.setPosition(sf::Vector2f(
        (windowSize.x - panelSize.x) / 2.0f,
        (windowSize.y - panelSize.y) / 2.0f
    ));
    
    title.setPosition(sf::Vector2f(popupPanel.getPosition().x + 20, popupPanel.getPosition().y + 15));
    closeButton.setPosition(
        popupPanel.getPosition().x + panelSize.x - 40, 
        popupPanel.getPosition().y + 15
    );

    tabManager.setPosition(popupPanel.getPosition().x + 30, popupPanel.getPosition().y + 70);
    tabManager.addTab("Thong Tin Ca Nhan");
    tabManager.addTab("Goi Tap Dang Ky");

    btnDangKyGoiMoi.setup("Dang Ky Goi Moi", font);
    btnDangKyGoiMoi.setSize(180, 40);
    btnDangKyGoiMoi.setPosition(
        popupPanel.getPosition().x + panelSize.x - 210,
        popupPanel.getPosition().y + panelSize.y - 60
    );
    
    // ✅ FIX: Mở CartPopup khi bấm
    btnDangKyGoiMoi.setOnClick([this]() {
    if (currentHoiVien == nullptr) return;
    
    std::cout << "🎫 Opening CartPopup for: " << currentHoiVien->getHoTen() << std::endl;
    
    // ✅ Mở CartPopup VỚI khách hàng đã chọn sẵn
    this->cartPopup.show(this->currentHoiVien, [this]() {
        std::cout << "✅ CartPopup closed, reloading data..." << std::endl;
        this->loadData();
    });
    
    this->hide();
    });
}

void HoiVienDetailPopup::show(HoiVien* hv) {
    if (hv == nullptr) {
        std::cerr << "❌ HoiVienDetailPopup::show() called with nullptr" << std::endl;
        return;
    }
    
    std::cout << "\n📋 ===== HIEN THI CHI TIET HOI VIEN =====" << std::endl;
    std::cout << "   HV: " << hv->getHoTen() << std::endl;
    std::cout << "   Point: " << hv->getPoint() << std::endl;
    std::cout << "   Rank: " << hv->getRank() << std::endl;
    std::cout << "   PT Sessions: " << hv->getSoBuoiPT() << std::endl;
    
    currentHoiVien = hv;
    title.setString("Chi Tiet: " + sf::String::fromUtf8(hv->getHoTen().begin(), hv->getHoTen().end()));
    
    loadData();
    
    std::cout << "========================================\n" << std::endl;
    BasePopup::show();
}

void HoiVienDetailPopup::hide() {
    BasePopup::hide();
    currentHoiVien = nullptr;
    infoLines.clear();
    packageRows.clear();
}

void HoiVienDetailPopup::loadData() {
    if (currentHoiVien == nullptr) return;
    
    std::cout << "\n🔄 ===== LOADING DATA =====" << std::endl;
    std::cout << "   Current HV: " << currentHoiVien->getHoTen() << std::endl;
    
    infoLines.clear();
    packageRows.clear();
    
    float pX = popupPanel.getPosition().x + 30;
    float pY = popupPanel.getPosition().y + 120;

    // Tab 1: Thông tin
    createInfoLine("ID", currentHoiVien->getID(), pY);
    createInfoLine("Ho Ten", currentHoiVien->getHoTen(), pY + 30);
    createInfoLine("So Dien Thoai", currentHoiVien->getSDT(), pY + 60);
    createInfoLine("Ngay Sinh", currentHoiVien->getNgaySinh(), pY + 90);
    createInfoLine("Gioi Tinh", currentHoiVien->getGioiTinh(), pY + 120);
    createInfoLine("Trang Thai", currentHoiVien->getIsActive() ? "Dang hoat dong" : "Da het han", pY + 150);
    createInfoLine("Rank", currentHoiVien->getRank(), pY + 180);
    createInfoLine("So buoi PT", std::to_string(currentHoiVien->getSoBuoiPT()), pY + 210);
    createInfoLine("Diem", std::to_string(currentHoiVien->getPoint()), pY + 240);

    // Tab 2: Gói tập
    MyVector<HopDong*>& dsHopDong = currentHoiVien->getDsHopDong();
    float rowY = pY;
    
    for (size_t i = 0; i < dsHopDong.size(); ++i) {
        HopDong* hd = dsHopDong[i];
        if (hd == nullptr) continue;

        const GoiTap& gt = hd->getGoiTap();
        bool isActive = hd->getIsActive(); // ✅ Kiểm tra còn hạn không
        
        PackageRowUI row(font);
        row.hopDong = hd;

        // ✅ Hiển thị thông tin gói
        std::string info = "Goi: " + gt.getTenGoi() + 
                           " | Het han: " + hd->getNgayHetHan() +
                           " | PT: " + std::to_string(hd->getSoBuoiPTConLai()) +
                           " | " + (isActive ? "Con han" : "Het han");
        row.info.setString(sf::String::fromUtf8(info.begin(), info.end()));
        row.info.setFillColor(isActive ? Config::TextNormal : Config::TextMuted);
        row.info.setPosition(sf::Vector2f(pX, rowY + 10));
        
        // ✅ NÚT 1: Gia Hạn (CHỈ hiện khi HẾT HẠN)
        if (!isActive) {
            std::string goiID = gt.getID();
            row.btnGiaHan.setup("Gia Han", font);
            row.btnGiaHan.setSize(80, 30);
            row.btnGiaHan.setPosition(pX + 540, rowY + 5); // ✅ CÙNG CỘT với Đặt HLV
            row.btnGiaHan.setOnClick([this, goiID]() {
                std::cout << "🔄 Renewing package: " << goiID << std::endl;
                
                this->cartPopup.show(this->currentHoiVien, goiID, [this]() {
                    this->loadData();
                });
                
                this->hide();
            });
        }

        // ✅ NÚT 2: Đặt HLV (CHỈ hiện khi CÒN HẠN + CÒN PT)
        if (isActive && hd->getSoBuoiPTConLai() > 0) {
            row.btnDatHLV.setup("Dat HLV", font);
            row.btnDatHLV.setSize(80, 30);
            row.btnDatHLV.setPosition(pX + 540, rowY + 5);
            row.btnDatHLV.setOnClick([this, hd]() {
                std::cout << "🏋️ Opening DatHLV popup..." << std::endl;
                
        // ✅ GỌI với callback reload
            this->datHLVPopup.show(this->currentHoiVien, hd, [this]() {
            std::cout << "🔄 DatHLV closed, reloading packages..." << std::endl;
            this->loadData(); // ✅ RELOAD DATA!
        });
    });
}
        
        packageRows.push_back(std::move(row));
        rowY += 45;
    }
    
    std::cout << "✅ Loaded " << packageRows.size() << " package rows" << std::endl;
    std::cout << "===========================\n" << std::endl;
}

void HoiVienDetailPopup::drawContent(sf::RenderTarget& target) {
    tabManager.draw(target);

    if (tabManager.getActiveTab() == 0) {
        drawTabInfo(target);
    } else {
        drawTabPackages(target);
    }
}

void HoiVienDetailPopup::drawTabInfo(sf::RenderTarget& target) {
    for (const auto& line : infoLines) {
        target.draw(line);
    }
}

void HoiVienDetailPopup::drawTabPackages(sf::RenderTarget& target) {
    for (auto& row : packageRows) {
        target.draw(row.info);
        row.btnGiaHan.draw(target);
        
        // ✅ Vẽ nút Đặt HLV nếu có callback
        if (row.btnDatHLV.getOnClick()) {
            row.btnDatHLV.draw(target);
        }
    }
    
    btnDangKyGoiMoi.draw(target);
}

void HoiVienDetailPopup::handleEvent(sf::Event event, sf::Vector2i mousePos) {
    if (!isVisible) return;
    BasePopup::handleEvent(event, mousePos);
    
    // ✅ Detect tab change
    int previousTab = tabManager.getActiveTab();
    tabManager.handleEvent(event, mousePos);
    
    // ✅ Nếu chuyển sang tab Gói Tập → Load lại
    if (previousTab != tabManager.getActiveTab() && tabManager.getActiveTab() == 1) {
        std::cout << "🔄 Switched to Packages tab, reloading..." << std::endl;
        loadData(); // ✅ Load lại để cập nhật gói tập mới nhất
    }

    if (tabManager.getActiveTab() == 1) {   
        for (auto& row : packageRows) {
            row.btnGiaHan.handleEvent(event, mousePos);
            if (row.btnDatHLV.getOnClick()) {
                row.btnDatHLV.handleEvent(event, mousePos);
            }
        }
        btnDangKyGoiMoi.handleEvent(event, mousePos);
    }
}

void HoiVienDetailPopup::update(sf::Vector2i mousePos) {
    if (!isVisible) return;
    BasePopup::update(mousePos);

    if (tabManager.getActiveTab() == 1) {
        for (auto& row : packageRows) {
            row.btnGiaHan.update(mousePos);
            if (row.btnDatHLV.getOnClick()) {
                row.btnDatHLV.update(mousePos);
            }
        }
        btnDangKyGoiMoi.update(mousePos);
    }
}

void HoiVienDetailPopup::createInfoLine(const std::string& label, const std::string& value, float yPos) {
    float labelX = popupPanel.getPosition().x + 50;
    float valueX = popupPanel.getPosition().x + 220;

    sf::Text labelText(font, sf::String::fromUtf8(label.begin(), label.end()) + ":", 16);
    labelText.setFillColor(Config::TextMuted);
    labelText.setPosition(sf::Vector2f(labelX, yPos));
    infoLines.push_back(labelText);

    sf::Text valueText(font, sf::String::fromUtf8(value.begin(), value.end()), 16);
    valueText.setFillColor(Config::TextNormal);
    valueText.setStyle(sf::Text::Bold);
    valueText.setPosition(sf::Vector2f(valueX, yPos));
    infoLines.push_back(valueText);
}