// GUI/Screens/BanHangScreen.cpp
#include "BanHangScreen.h"
#include "Managers/QuanLy.h"
#include "Services/HoaDonService.h" // Goi Service (neu can xoa)
#include "DateTimeUils.h" 
#include "Validator.h"    

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

    tuNgayInput.setup("Tu ngay (DD/MM/YYYY)", app.getGlobalFont(), false);
    tuNgayInput. setSize(180, 35);
    tuNgayInput.setPosition(contentX, 40);
    
    denNgayInput.setup("Den ngay (DD/MM/YYYY)", app.getGlobalFont(), false);
    denNgayInput.setSize(180, 35);
    denNgayInput.setPosition(contentX + 190, 40);
    
    locButton.setup("Loc", app.getGlobalFont());
    locButton.setSize(100, 35);
    locButton.setPosition(contentX + 380, 40);
    locButton.setOnClick([this]() {
        this->applyFilter();
    });
    
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
    
    std::string today = DateTimeUtils::getCurrentDateTime();
    tuNgayInput. setString("01/" + today. substr(3, 7)); // "01/MM/YYYY"
    denNgayInput.setString(today.substr(0, 10));       // "DD/MM/YYYY"
    
    loadAndDisplayData();
}

void BanHangScreen::applyFilter() {
    allPaidHoaDon. clear();
    
    std::string tuNgay = tuNgayInput.getString();
    std::string denNgay = denNgayInput.getString();
    
    // Validate ngày
    bool hasFilter = false;
    if (!tuNgay.empty() && ! denNgay.empty()) {
        if (Validator::validateNgay(tuNgay) == "" && Validator::validateNgay(denNgay) == "") {
            hasFilter = true;
        }
    }
    
    // Lấy tất cả hóa đơn
    MyVector<HoaDon*> tatCaHoaDon = app.getQuanLy().getDsHoaDon(). getAllValues();
    
    for (size_t i = 0; i < tatCaHoaDon.size(); ++i) {
        HoaDon* hd = tatCaHoaDon[i];
        if (hd == nullptr || ! hd->getDaThanhToan()) continue;
        
        // ✅ Lọc theo ngày
        if (hasFilter) {
            std::string ngayLap = hd->getNgayLap();
            if (! DateTimeUtils::isBetween(ngayLap, tuNgay, denNgay)) {
                continue;
            }
        }
        
        allPaidHoaDon.push_back(hd);
    }
    
    std::cout << "🔍 Filtered " << allPaidHoaDon.size() << " invoices" << std::endl;
    
    pagination.setup(allPaidHoaDon.size(), 10);
    onPageChange(pagination.getCurrentPage());
}

// --- Ham Logic (DA DON GIAN HOA) ---
void BanHangScreen::loadAndDisplayData() {
    applyFilter();
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
    sf::Vector2i mousePos = sf::Mouse::getPosition(app. getWindow());

    // Xử lý Popups trước
    if (cartPopup.getIsVisible()) { cartPopup.handleEvent(event, mousePos); return; }
    if (detailPopup.getIsVisible()) { detailPopup.handleEvent(event, mousePos); return; }
    if (deletePopup.getIsVisible()) { deletePopup.handleEvent(event, mousePos); return; }

    // ✅ XỬ LÝ CLICK VÀO INPUT BOX
    if (auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            tuNgayInput. setFocus(tuNgayInput.isMouseOver(mousePos));
            denNgayInput.setFocus(denNgayInput.isMouseOver(mousePos));
        }
    }
    
    // ✅ XỬ LÝ NHẬP TEXT VÀO INPUT BOX
    if (event.getIf<sf::Event::TextEntered>()) {
        if (tuNgayInput.getFocus()) {
            tuNgayInput.handleEvent(event);
        } else if (denNgayInput. getFocus()) {
            denNgayInput.handleEvent(event);
        }
    }
    
    // ✅ XỬ LÝ PHÍM (Mũi tên, Backspace, Enter...)
    if (auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        if (tuNgayInput.getFocus()) {
            tuNgayInput.handleEvent(event);
            if (keyEvent->code == sf::Keyboard::Key::Enter) {
                applyFilter(); // ✅ Enter → Lọc
            }
        } else if (denNgayInput.getFocus()) {
            denNgayInput.handleEvent(event);
            if (keyEvent->code == sf::Keyboard::Key::Enter) {
                applyFilter();
            }
        }
    }
    
    // Xử lý nút lọc
    locButton.handleEvent(event, mousePos);
    
    // Xử lý màn hình chính
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

    tuNgayInput.update(dt);
    denNgayInput.update(dt);
    locButton.update(mousePos);

    // Update man hinh chinh
    themHoaDonButton.update(mousePos);
    hoaDonTable.update(mousePos);
    pagination.update(mousePos);
}

void BanHangScreen::draw(sf::RenderTarget& target) {
    tuNgayInput.draw(target);
    denNgayInput.draw(target);
    locButton.draw(target);
    
    // 1. Ve man hinh chinh
    themHoaDonButton.draw(target);
    hoaDonTable.draw(target);
    pagination.draw(target);
    
    // 2. Ve Popups (phai ve CUOI CUNG)
    cartPopup.draw(target);
    detailPopup.draw(target);
    deletePopup.draw(target);
}