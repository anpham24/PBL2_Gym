// GUI/Screens/HangHoaScreen.cpp
#include "HangHoaScreen.h"
#include "Managers/QuanLy.h"
#include "Services/HangHoaService.h" // Goi Service

// --- Constructor (DA CAP NHAT QUYEN) ---
HangHoaScreen::HangHoaScreen(App& app) 
    : BaseScreen(app), 
      hangHoaTable(app.getGlobalFont()), 
      pagination(app.getGlobalFont()),
      formPopup(app),                 // Khoi tao popup
      deletePopup(app)                // Khoi tao popup
{
    // --- (MOI) KIEM TRA QUYEN ---
    isStaffReadOnly = (app.getCurrentAccount()->getAccountType() == AccountType::STAFF);

    float contentX = 250.0f; 
    
    // --- Nut Them ---
    themHangHoaButton.setup("Them Hang Hoa Moi", app.getGlobalFont());
    themHangHoaButton.setSize(200, 40);
    themHangHoaButton.setPosition(app.getWindow().getSize().x - 230, 40);
    themHangHoaButton.setOnClick([this]() {
        this->formPopup.show(nullptr, [this]() {
            this->loadAndDisplayData(); // Callback load lai data
        });
    });

    // --- Bang Du Lieu ---
    hangHoaTable.setPosition(contentX, 100);
    
    // Cac cot luon hien thi
    hangHoaTable.addColumn("ID", 100, [](const HangHoa* hh) { return hh->getID(); });
    hangHoaTable.addColumn("Ten Hang Hoa", 250, [](const HangHoa* hh) { return hh->getTenHH(); });
    hangHoaTable.addColumn("Loai", 150, [](const HangHoa* hh) { return hh->getLoaiHH(); });
    hangHoaTable.addColumn("Gia", 150, [](const HangHoa* hh) { return std::to_string((int)hh->getGia()); });
    hangHoaTable.addColumn("So Luong Con", 120, [](const HangHoa* hh) { return std::to_string(hh->getSoLuongCon()); });
    
    // --- (MOI) CHI ADMIN MOI THAY COT SUA/XOA ---
    if (!isStaffReadOnly) {
        // Cot "Sua"
        hangHoaTable.addEditAction([this](HangHoa* hh) {
            this->formPopup.show(hh, [this]() {
                this->loadAndDisplayData();
            });
        });

        // Cot "Xoa"
        hangHoaTable.addDeleteAction([this](HangHoa* hh) {
            auto onConfirm = [this, hh]() {
                HangHoaService::xoaHangHoa(hh->getID()); // GOI SERVICE
                this->loadAndDisplayData();
            };
            
            // Kiem tra rang buoc
            std::string warning = "";
            if (!hh->getDsChiTietHoaDon_HH().empty()) {
                warning = "Hang hoa nay da co trong lich su Hoa Don!";
            }
            this->deletePopup.show(hh->getTenHH(), onConfirm, warning);
        });
    }

    // --- Phan Trang ---
    pagination.setPosition(contentX, 600);
    pagination.setOnPageChange([this](int newPage) {
        this->onPageChange(newPage);
    });
    
    loadAndDisplayData();
}

// --- Ham Logic ---
void HangHoaScreen::loadAndDisplayData() {
    allHangHoa.clear();
    
    MyVector<HangHoa*>& dsHangHoaGoc = app.getQuanLy().getDsHangHoa();
    for (size_t i = 0; i < dsHangHoaGoc.size(); ++i) {
        allHangHoa.push_back(dsHangHoaGoc[i]);
    }
    
    // (Sau nay se them logic Tim kiem/Loc o day)
    
    pagination.setup(allHangHoa.size(), 10);
    onPageChange(pagination.getCurrentPage());
}

void HangHoaScreen::onPageChange(int newPage) {
    pagedHangHoa.clear();
    int startIndex = (newPage - 1) * 10;
    int endIndex = std::min(startIndex + 10, (int)allHangHoa.size());
    
    for (int i = startIndex; i < endIndex; ++i) {
        pagedHangHoa.push_back(allHangHoa.at(i));
    }
    
    hangHoaTable.setData(pagedHangHoa);
}

// --- Vong lap chinh (DA CAP NHAT QUYEN) ---
void HangHoaScreen::handleEvent(sf::Event event) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(app.getWindow());

    // Xu ly Popups truoc
    if (formPopup.getIsVisible()) { formPopup.handleEvent(event, mousePos); return; }
    if (deletePopup.getIsVisible()) { deletePopup.handleEvent(event, mousePos); return; }
    
    // Xu ly man hinh chinh
    if (!isStaffReadOnly) { 
        themHangHoaButton.handleEvent(event, mousePos);
    }
    hangHoaTable.handleEvent(event, mousePos);
    pagination.handleEvent(event, mousePos);
}

void HangHoaScreen::update(sf::Time dt) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(app.getWindow());

    // Luon update cac popup
    formPopup.update(mousePos);
    deletePopup.update(mousePos);
    
    if (formPopup.getIsVisible() || deletePopup.getIsVisible()) {
        return; 
    }

    // Update man hinh chinh
    if (!isStaffReadOnly) {
        themHangHoaButton.update(mousePos);
    }
    hangHoaTable.update(mousePos);
    pagination.update(mousePos);
}

void HangHoaScreen::draw(sf::RenderTarget& target) {
    // 1. Ve man hinh chinh
    if (!isStaffReadOnly) { // Staff khong thay nut "Them"
        themHangHoaButton.draw(target);
    }
    hangHoaTable.draw(target);
    pagination.draw(target);
    
    // 2. Ve Popups (phai ve CUOI CUNG)
    formPopup.draw(target);
    deletePopup.draw(target);
}