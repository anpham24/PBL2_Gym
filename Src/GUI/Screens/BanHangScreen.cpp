// GUI/Screens/BanHangScreen.cpp
#include "BanHangScreen.h"
#include "Managers/QuanLy.h"
#include "Services/HoaDonService.h" // Goi Service (neu can xoa)

// --- Constructor (DA DON GIAN HOA) ---
BanHangScreen::BanHangScreen(App& app) 
    : BaseScreen(app), 
      hoaDonTable(app.getGlobalFont()), 
      pagination(app.getGlobalFont()),
      cartPopup(app),                 // Khoi tao popup
      detailPopup(app),               // Khoi tao popup
      deletePopup(app)                // Khoi tao popup
{
    float contentX = 250.0f; 
    
    // --- Nut Them ---
    themHoaDonButton.setup("Tao Hoa Don Moi", app.getGlobalFont());
    themHoaDonButton.setSize(200, 40);
    themHoaDonButton.setPosition(app.getWindow().getSize().x - 230, 40);
    themHoaDonButton.setOnClick([this]() {
        // Mo popup Cart o che do "Them moi"
        // (Ham show() cua CartPopup gio khong can tham so HoaDon*)
        this->cartPopup.show([this]() {
            this->loadAndDisplayData(); // Load lai lich su sau khi thanh toan
        });
    });

    // --- Bang Du Lieu (Chi la LICH SU) ---
    hoaDonTable.setPosition(contentX, 100);
    
    // Cac cot luon hien thi
    hoaDonTable.addColumn("ID Hoa Don", 150, [](const HoaDon* hd) { return hd->getID(); });
    hoaDonTable.addColumn("Khach Hang", 200, [](const HoaDon* hd) { 
        return (hd->getHoiVien() != nullptr) ? hd->getHoiVien()->getHoTen() : "Khach le"; 
    });
    hoaDonTable.addColumn("Nhan Vien", 200, [](const HoaDon* hd) { 
        return (hd->getNhanVien() != nullptr) ? hd->getNhanVien()->getHoTen() : "N/A"; 
    });
    hoaDonTable.addColumn("Ngay Lap", 150, [](const HoaDon* hd) { return hd->getNgayLap(); });
    hoaDonTable.addColumn("Tong Tien", 120, [](const HoaDon* hd) { return std::to_string((int)hd->getFinalTotal());});    
    // Cot hanh dong chi co "Xem"
    hoaDonTable.addAction("Xem Chi Tiet", [this](HoaDon* hd) {
        this->detailPopup.show(hd);
    }, Config::AccentCyan, 120);
    
    // (Admin co the xoa hoa don khoi lich su)
    if (app.getCurrentAccount()->getAccountType() == AccountType::ADMIN) {
        hoaDonTable.addDeleteAction([this](HoaDon* hd) {
            auto onConfirm = [this, hd]() {
                // (Luu y: Ban can ham Service de xoa vinh vien HoaDon)
                // HoaDonService::xoaVinhVien(hd->getID()); 
                this->loadAndDisplayData();
            };
            this->deletePopup.show("Hoa don " + hd->getID(), onConfirm, "Hanh dong nay se xoa vinh vien hoa don.");
        });
    }

    // --- Phan Trang ---
    pagination.setPosition(contentX, 600);
    pagination.setOnPageChange([this](int newPage) {
        this->onPageChange(newPage);
    });
    
    loadAndDisplayData();
}

// --- Ham Logic (DA DON GIAN HOA) ---
void BanHangScreen::loadAndDisplayData() {
    allPaidHoaDon.clear(); 
    
    // Lay data tu QuanLy (HoaDon dung HashTable)
    MyVector<HoaDon*> tatCaHoaDon = app.getQuanLy().getDsHoaDon().getAllValues();
    
    // Chi loc cac hoa don DA THANH TOAN
    for(size_t i = 0; i < tatCaHoaDon.size(); ++i) {
        HoaDon* hd = tatCaHoaDon[i];
        if (hd->getDaThanhToan()) { // <-- LOGIC MOI
            // (Them loc theo ngay/tim kiem o day)
            allPaidHoaDon.push_back(hd);
        }
    }
    
    pagination.setup(allPaidHoaDon.size(), 10);
    onPageChange(pagination.getCurrentPage());
}

void BanHangScreen::onPageChange(int newPage) {
    pagedHoaDon.clear();
    int startIndex = (newPage - 1) * 10;
    int endIndex = std::min(startIndex + 10, (int)allPaidHoaDon.size());
    
    for (int i = startIndex; i < endIndex; ++i) {
        pagedHoaDon.push_back(allPaidHoaDon.at(i));
    }
    
    hoaDonTable.setData(pagedHoaDon);
}

// --- Vong lap chinh (DA DON GIAN HOA) ---
void BanHangScreen::handleEvent(sf::Event event) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(app.getWindow());

    // Xu ly Popups truoc
    if (cartPopup.getIsVisible()) { cartPopup.handleEvent(event, mousePos); return; }
    if (detailPopup.getIsVisible()) { detailPopup.handleEvent(event, mousePos); return; }
    if (deletePopup.getIsVisible()) { deletePopup.handleEvent(event, mousePos); return; }
    
    // Xu ly man hinh chinh
    themHoaDonButton.handleEvent(event, mousePos);
    hoaDonTable.handleEvent(event, mousePos);
    pagination.handleEvent(event, mousePos);
}

void BanHangScreen::update(sf::Time dt) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(app.getWindow());

    // Luon update cac popup
    cartPopup.update(mousePos);
    detailPopup.update(mousePos);
    deletePopup.update(mousePos);
    
    if (cartPopup.getIsVisible() || detailPopup.getIsVisible() || deletePopup.getIsVisible()) {
        return; 
    }

    // Update man hinh chinh
    themHoaDonButton.update(mousePos);
    hoaDonTable.update(mousePos);
    pagination.update(mousePos);
}

void BanHangScreen::draw(sf::RenderTarget& target) {
    // 1. Ve man hinh chinh
    themHoaDonButton.draw(target);
    hoaDonTable.draw(target);
    pagination.draw(target);
    
    // 2. Ve Popups (phai ve CUOI CUNG)
    cartPopup.draw(target);
    detailPopup.draw(target);
    deletePopup.draw(target);
}