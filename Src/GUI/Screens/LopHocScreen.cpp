// GUI/Screens/LopHocScreen.cpp
#include "LopHocScreen.h"
#include "Managers/QuanLy.h"
#include "Services/LopHocService.h" // Goi Service

// --- Constructor (DA CAP NHAT QUYEN) ---
LopHocScreen::LopHocScreen(App& app) 
    : BaseScreen(app), 
      lopHocTable(app.getGlobalFont()), 
      pagination(app.getGlobalFont()),
      formPopup(app),                 // Khoi tao popup
      deletePopup(app)                // Khoi tao popup
{
    // --- (MOI) KIEM TRA QUYEN ---
    isStaffReadOnly = (app.getCurrentAccount()->getAccountType() == AccountType::STAFF);

    float contentX = 250.0f; 
    
    // --- Nut Them ---
    themLopHocButton.setup("Them Lop Hoc Moi", app.getGlobalFont());
    themLopHocButton.setSize(200, 40);
    themLopHocButton.setPosition(app.getWindow().getSize().x - 230, 40);
    themLopHocButton.setOnClick([this]() {
        this->formPopup.show(nullptr, [this]() {
            this->loadAndDisplayData(); // Callback load lai data
        });
    });

    // --- Bang Du Lieu ---
    lopHocTable.setPosition(contentX, 100);
    
    // Cac cot luon hien thi
    lopHocTable.addColumn("ID", 100, [](const LopHoc* lh) { return lh->getID(); });
    lopHocTable.addColumn("Ten Lop", 200, [](const LopHoc* lh) { return lh->getTenLop(); });
    lopHocTable.addColumn("Mon Tap", 150, [](const LopHoc* lh) {
        return (lh->getMonTap() != nullptr) ? lh->getMonTap()->getTenMon() : "N/A";
    });
    lopHocTable.addColumn("HLV", 150, [](const LopHoc* lh) {
        return (lh->getHLV() != nullptr) ? lh->getHLV()->getHoTen() : "N/A";
    });
    lopHocTable.addColumn("Lich Tap", 150, [](const LopHoc* lh) { return lh->getLichTap(); });
    lopHocTable.addColumn("Thoi Luong", 100, [](const LopHoc* lh) { return std::to_string(lh->getThoiLuong()) + " phut"; });
    
    // --- (MOI) CHI ADMIN MOI THAY COT SUA/XOA ---
    if (!isStaffReadOnly) {
        // Cot "Sua"
        lopHocTable.addEditAction([this](LopHoc* lh) {
            this->formPopup.show(lh, [this]() {
                this->loadAndDisplayData();
            });
        });

        // Cot "Xoa"
        lopHocTable.addDeleteAction([this](LopHoc* lh) {
            auto onConfirm = [this, lh]() {
                LopHocService::xoaLopHoc(lh->getID()); // GOI SERVICE
                this->loadAndDisplayData();
            };
            // Lop Hoc khong co rang buoc phu thuoc phuc tap
            this->deletePopup.show(lh->getTenLop(), onConfirm);
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
void LopHocScreen::loadAndDisplayData() {
    allLopHoc.clear();
    
    const MyVector<LopHoc*>& dsLopHocGoc = app.getQuanLy().getDsLopHoc();
    for (size_t i = 0; i < dsLopHocGoc.size(); ++i) {
        allLopHoc.push_back(dsLopHocGoc[i]);
    }
    
    // (Sau nay se them logic Tim kiem/Loc o day)
    
    pagination.setup(allLopHoc.size(), 10);
    onPageChange(pagination.getCurrentPage());
}

void LopHocScreen::onPageChange(int newPage) {
    pagedLopHoc.clear();
    int startIndex = (newPage - 1) * 10;
    int endIndex = std::min(startIndex + 10, (int)allLopHoc.size());
    
    for (int i = startIndex; i < endIndex; ++i) {
        pagedLopHoc.push_back(allLopHoc.at(i));
    }
    
    lopHocTable.setData(pagedLopHoc);
}

// --- Vong lap chinh (DA CAP NHAT QUYEN) ---
void LopHocScreen::handleEvent(sf::Event event) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(app.getWindow());

    // Xu ly Popups truoc
    if (formPopup.getIsVisible()) { formPopup.handleEvent(event, mousePos); return; }
    if (deletePopup.getIsVisible()) { deletePopup.handleEvent(event, mousePos); return; }
    
    // Xu ly man hinh chinh
    if (!isStaffReadOnly) { // Staff khong the an nut "Them"
        themLopHocButton.handleEvent(event, mousePos);
    }
    lopHocTable.handleEvent(event, mousePos);
    pagination.handleEvent(event, mousePos);
}

void LopHocScreen::update(sf::Time dt) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(app.getWindow());

    // Luon update cac popup
    formPopup.update(mousePos);
    deletePopup.update(mousePos);
    
    if (formPopup.getIsVisible() || deletePopup.getIsVisible()) {
        return; 
    }

    // Update man hinh chinh
    if (!isStaffReadOnly) {
        themLopHocButton.update(mousePos);
    }
    lopHocTable.update(mousePos);
    pagination.update(mousePos);
}

void LopHocScreen::draw(sf::RenderTarget& target) {
    // 1. Ve man hinh chinh
    if (!isStaffReadOnly) { // Staff khong thay nut "Them"
        themLopHocButton.draw(target);
    }
    lopHocTable.draw(target);
    pagination.draw(target);
    
    // 2. Ve Popups (phai ve CUOI CUNG)
    formPopup.draw(target);
    deletePopup.draw(target);
}