// GUI/Popups/HoiVienDetailPopup.cpp
#include "HoiVienDetailPopup.h"
#include <iostream>

HoiVienDetailPopup::HoiVienDetailPopup(App& app)
    : BasePopup(app, "Chi Tiet Hoi Vien"), // Tieu de
     font(app.getGlobalFont()),
      tabManager(app.getGlobalFont()),
      currentHoiVien(nullptr),
      btnDangKyGoiMoi()
{
    // Mo rong kich thuoc popup de co nhieu khong gian hon
    sf::Vector2f panelSize(700, 500);
    popupPanel.setSize(panelSize);
    
    // Dat lai vi tri panel
    sf::Vector2u windowSize = app.getWindow().getSize();
    popupPanel.setPosition(sf::Vector2f(
        (windowSize.x - panelSize.x) / 2.0f,
        (windowSize.y - panelSize.y) / 2.0f
    ));
    
    // Dat lai vi tri title va nut [X]
    title.setPosition(sf::Vector2f(popupPanel.getPosition().x + 20, popupPanel.getPosition().y + 15));
    closeButton.setPosition(
        popupPanel.getPosition().x + panelSize.x - 40, 
        popupPanel.getPosition().y + 15
    );

    // --- Setup TabManager ---
    tabManager.setPosition(popupPanel.getPosition().x + 30, popupPanel.getPosition().y + 70);
    tabManager.addTab("Thong Tin Ca Nhan");
    tabManager.addTab("Goi Tap Dang Ky");

    // --- Setup nut "Dang Ky Goi Moi" (o Tab 2) ---
    btnDangKyGoiMoi.setup("Dang Ky Goi Moi", font);
    btnDangKyGoiMoi.setSize(180, 40);
    btnDangKyGoiMoi.setPosition(
        popupPanel.getPosition().x + panelSize.x - 210, // Ben phai
        popupPanel.getPosition().y + panelSize.y - 60   // Ben duoi
    );
    
    // Gan chuc nang: Chuyen sang man hinh Ban Hang
    btnDangKyGoiMoi.setOnClick([this]() {
        if (currentHoiVien == nullptr) return;
        std::cout << "Chuyen qua man hinh Ban Hang voi HV: " 
                  << currentHoiVien->getHoTen() << std::endl;
        
        // (Sau nay se goi ham chuyen man hinh kem theo data)
        // app.getNavigation().navigateTo(ContentScreenType::BanHang, currentHoiVien);
        
        this->hide();
    });
}

void HoiVienDetailPopup::show(HoiVien* hv) {
    if (hv == nullptr) {
        std::cerr << "Loi (HoiVienDetailPopup): show() duoc goi voi nullptr." << std::endl;
        return;
    }
    currentHoiVien = hv;
    title.setString("Chi Tiet: " + sf::String::fromUtf8(hv->getHoTen().begin(), hv->getHoTen().end()));
    
    loadData(); // Nap du lieu vao cac component
    
    BasePopup::show();
}

void HoiVienDetailPopup::hide() {
    BasePopup::hide();
    currentHoiVien = nullptr; // Reset
    infoLines.clear();
    packageRows.clear();
}

void HoiVienDetailPopup::loadData() {
    if (currentHoiVien == nullptr) return;
    
    // Xoa du lieu cu
    infoLines.clear();
    packageRows.clear();
    
    float pX = popupPanel.getPosition().x + 30;
    float pY = popupPanel.getPosition().y + 120; // Vi tri bat dau noi dung (duoi tabs)

    // --- Load Tab 1: Thong Tin Ca Nhan ---
    createInfoLine("ID", currentHoiVien->getID(), pY);
    createInfoLine("Ho Ten", currentHoiVien->getHoTen(), pY + 30);
    createInfoLine("So Dien Thoai", currentHoiVien->getSDT(), pY + 60);
    createInfoLine("Ngay Sinh", currentHoiVien->getNgaySinh(), pY + 90);
    createInfoLine("Gioi Tinh", currentHoiVien->getGioiTinh(), pY + 120);
    createInfoLine("Trang Thai", currentHoiVien->getIsActive() ? "Dang hoat dong" : "Da het han", pY + 150);
    // (Ban tu them logic lay Rank)
    createInfoLine("Rank", "Bronze (Sample)", pY + 180);
    createInfoLine("So buoi PT con lai", std::to_string(currentHoiVien->getSoBuoiPT()), pY + 210);

    // --- Load Tab 2: Cac Goi Tap ---
    MyVector<HopDong*>& dsHopDong = currentHoiVien->getDsHopDong();
    float rowY = pY; // Vi tri Y cho hang dau tien cua goi tap

    for (size_t i = 0; i < dsHopDong.size(); ++i) {
        HopDong* hd = dsHopDong[i];
        if (hd == nullptr) continue;

        PackageRowUI row(font);
        row.hopDong = hd;
        const GoiTap& gt = hd->getGoiTap(); // GoiTap.h can ham getGoiTap() tra ve const&

        // Thong tin goi tap
        std::string info = "Goi: " + gt.getTenGoi() + 
                           " (Het han: " + hd->getNgayHetHan() + ")";
        row.info.setString(sf::String::fromUtf8(info.begin(), info.end()));
        row.info.setFillColor(Config::TextNormal);
        row.info.setPosition(sf::Vector2f(pX, rowY + 10));
        
        // Nut Gia Han
        row.btnGiaHan.setup("Gia Han", font);
        row.btnGiaHan.setSize(80, 30);
        row.btnGiaHan.setPosition(pX + 450, rowY + 5);
        row.btnGiaHan.setOnClick([this, &gt]() {
             std::cout << "Gia han goi: " << gt.getTenGoi() << std::endl;
             // (Logic chuyen sang man hinh Ban Hang kem theo Goi Tap)
             this->hide();
        });

        // Nut Dat HLV (chi hien khi con buoi PT)
        if (currentHoiVien->getSoBuoiPT() > 0) {
            row.btnDatHLV.setup("Dat HLV", font);
            row.btnDatHLV.setSize(80, 30);
            row.btnDatHLV.setPosition(pX + 540, rowY + 5);
            row.btnDatHLV.setOnClick([this, &gt]() {
                std::cout << "Dat HLV cho goi: " << gt.getTenGoi() << std::endl;
                // (Goi DatHLVPopup)
                // datHLVPopup.show(currentHoiVien, gt.getDsMonTap());
            });
        }
        
        packageRows.push_back(std::move(row)); // Chuyen data vao vector
        rowY += 45; // Chieu cao moi hang
    }
}

// --- Ve noi dung theo Tab ---
void HoiVienDetailPopup::drawContent(sf::RenderTarget& target) {
    tabManager.draw(target); // Ve thanh Tab

    if (tabManager.getActiveTab() == 0) {
        drawTabInfo(target);
    } else {
        drawTabPackages(target);
    }
}

void HoiVienDetailPopup::drawTabInfo(sf::RenderTarget& target) {
    // Ve cac dong thong tin da tao trong loadData()
    for (const auto& line : infoLines) {
        target.draw(line);
    }
}

void HoiVienDetailPopup::drawTabPackages(sf::RenderTarget& target) {
    // Ve cac hang goi tap
    for (auto& row : packageRows) {
        target.draw(row.info);
        row.btnGiaHan.draw(target);
        
        // Chi ve nut "Dat HLV" neu no duoc khoi tao (con buoi PT)
        // (Kiem tra xem callback co ton tai khong)
        if (row.btnDatHLV.getOnClick()) {
            row.btnDatHLV.draw(target);
        }
    }
    
    // Ve nut "Dang Ky Goi Moi"
    btnDangKyGoiMoi.draw(target);
}

// --- Xu ly Event & Update ---
void HoiVienDetailPopup::handleEvent(sf::Event event, sf::Vector2i mousePos) {
    if (!isVisible) return;
    BasePopup::handleEvent(event, mousePos); // Xu ly nut [X]
    tabManager.handleEvent(event, mousePos); // Xu ly click tab

    if (tabManager.getActiveTab() == 1) {
        // Neu o Tab 2, xu ly cac nut trong do
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
    BasePopup::update(mousePos); // Cap nhat nut [X]

    if (tabManager.getActiveTab() == 1) {
        // Cap nhat trang thai hover cho cac nut o Tab 2
        for (auto& row : packageRows) {
            row.btnGiaHan.update(mousePos);
            if (row.btnDatHLV.getOnClick()) {
                row.btnDatHLV.update(mousePos);
            }
        }
        btnDangKyGoiMoi.update(mousePos);
    }
}

/**
 * @brief Ham Helper: Tao va them 2 sf::Text (label va value) vao vector infoLines.
 */
void HoiVienDetailPopup::createInfoLine(const std::string& label, const std::string& value, float yPos) {
    float labelX = popupPanel.getPosition().x + 50;
    float valueX = popupPanel.getPosition().x + 220;

    // Tao Label (mau mo)
    sf::Text labelText(font, sf::String::fromUtf8(label.begin(), label.end()) + ":", 16);
    labelText.setFillColor(Config::TextMuted);
    labelText.setPosition(sf::Vector2f(labelX, yPos));
    infoLines.push_back(labelText);

    // Tao Value (mau sang, dam)
    sf::Text valueText(font, sf::String::fromUtf8(value.begin(), value.end()), 16);
    valueText.setFillColor(Config::TextNormal);
    valueText.setStyle(sf::Text::Bold);
    valueText.setPosition(sf::Vector2f(valueX, yPos));
    infoLines.push_back(valueText);
}