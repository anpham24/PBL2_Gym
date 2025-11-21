// GUI/Screens/GoiTapScreen.cpp
#include "GoiTapScreen.h"
#include "Managers/QuanLy.h"
#include "Services/GoiTapService.h"

// --- Constructor (DA CAP NHAT QUYEN) ---
GoiTapScreen::GoiTapScreen(App& app) 
    : BaseScreen(app), 
      goiTapTable(app.getGlobalFont()), 
      pagination(app.getGlobalFont()),
      formPopup(app),
      monTapPopup(app),
      deletePopup(app)
{
    // --- (MOI) KIEM TRA QUYEN ---
    isStaffReadOnly = (app.getCurrentAccount()->getAccountType() == AccountType::STAFF);

    float contentX = 250.0f; 
    
    // --- Nut Them ---
    themGoiTapButton.setup("Them Goi Tap Moi", app.getGlobalFont());
    themGoiTapButton.setSize(200, 40);
    themGoiTapButton.setPosition(app.getWindow().getSize().x - 230, 40);
    themGoiTapButton.setOnClick([this]() {
        this->formPopup.show(nullptr, [this]() {
            this->loadAndDisplayData();
        });
    });

    // --- Bang Du Lieu ---
    goiTapTable.setPosition(contentX, 100);
    
    // Cac cot luon hien thi
    goiTapTable.addColumn("ID", 100, [](const GoiTap* gt) { return gt->getID(); });
    goiTapTable.addColumn("Ten Goi", 230, [](const GoiTap* gt) { return gt->getTenGoi(); });
    goiTapTable.addColumn("Thoi Gian (Ngay)", 150, [](const GoiTap* gt) { return std::to_string(gt->getThoiGian()); });
    goiTapTable.addColumn("Gia", 150, [](const GoiTap* gt) { return std::to_string((int)gt->getGia()); });
    goiTapTable.addColumn("Buoi PT", 80, [](const GoiTap* gt) { return std::to_string(gt->getSoBuoiPT()); });
    
    // Cot "Mon Tap" (Nut "Xem" - Luon hien thi cho Staff)
    goiTapTable.addAction("Xem Mon", [this](GoiTap* gt) {
        // (SUA DOI) Truyen co `isStaffReadOnly` vao popup
        this->monTapPopup.show(gt, isStaffReadOnly); 
    }, Config::AccentCyan, 100);
    
    // --- (MOI) CHI ADMIN MOI THAY COT SUA/XOA ---
    if (!isStaffReadOnly) {
        // Cot "Sua"
        goiTapTable.addEditAction([this](GoiTap* gt) {
            this->formPopup.show(gt, [this]() {
                this->loadAndDisplayData();
            });
        });

        // Cot "Xoa"
        goiTapTable.addDeleteAction([this](GoiTap* gt) {
            auto onConfirm = [this, gt]() {
                GoiTapService::xoaGoiTap(gt->getID()); // GOI SERVICE
                this->loadAndDisplayData();
            };
            
            std::string warning = "";
            if (!gt->getDsHopDong().empty() || !gt->getDsChiTietHoaDon_GT().empty()) {
                warning = "Goi tap nay dang duoc su dung (Hop Dong / Hoa Don)!";
            }
            this->deletePopup.show(gt->getTenGoi(), onConfirm, warning);
        });
    }

    // --- Phan Trang ---
    pagination.setPosition(contentX, 600);
    pagination.setOnPageChange([this](int newPage) {
        this->onPageChange(newPage);
    });
    
    loadAndDisplayData();
}

// --- Ham Logic (DA CAP NHAT) ---
void GoiTapScreen::loadAndDisplayData() {
    allGoiTap.clear();
    
    const MyVector<GoiTap*>& dsGoiTapGoc = app.getQuanLy().getDsGoiTap();
    for (size_t i = 0; i < dsGoiTapGoc.size(); ++i) {
        allGoiTap.push_back(dsGoiTapGoc[i]);
    }
    
    pagination.setup(allGoiTap.size(), 10);
    onPageChange(pagination.getCurrentPage());
}

void GoiTapScreen::onPageChange(int newPage) {
    pagedGoiTap.clear();
    int startIndex = (newPage - 1) * 10;
    int endIndex = std::min(startIndex + 10, (int)allGoiTap.size());
    
    for (int i = startIndex; i < endIndex; ++i) {
        pagedGoiTap.push_back(allGoiTap.at(i));
    }
    
    goiTapTable.setData(pagedGoiTap);
}

// --- Vong lap chinh (DA CAP NHAT QUYEN) ---
void GoiTapScreen::handleEvent(sf::Event event) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(app.getWindow());

    // Xu ly Popups truoc
    if (formPopup.getIsVisible()) { formPopup.handleEvent(event, mousePos); return; }
    if (monTapPopup.getIsVisible()) { monTapPopup.handleEvent(event, mousePos); return; }
    if (deletePopup.getIsVisible()) { deletePopup.handleEvent(event, mousePos); return; }
    
    // Xu ly man hinh chinh
    if (!isStaffReadOnly) { // Staff khong the an nut "Them"
        themGoiTapButton.handleEvent(event, mousePos);
    }
    goiTapTable.handleEvent(event, mousePos);
    pagination.handleEvent(event, mousePos);
}

void GoiTapScreen::update(sf::Time dt) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(app.getWindow());

    // Luon update cac popup
    formPopup.update(mousePos);
    monTapPopup.update(mousePos);
    deletePopup.update(mousePos);
    
    if (formPopup.getIsVisible() || monTapPopup.getIsVisible() || deletePopup.getIsVisible()) {
        return; 
    }

    // Update man hinh chinh
    if (!isStaffReadOnly) { // Staff khong update nut "Them"
        themGoiTapButton.update(mousePos);
    }
    goiTapTable.update(mousePos);
    pagination.update(mousePos);
}

void GoiTapScreen::draw(sf::RenderTarget& target) {
    // 1. Ve man hinh chinh
    if (!isStaffReadOnly) { // Staff khong thay nut "Them"
        themGoiTapButton.draw(target);
    }
    goiTapTable.draw(target);
    pagination.draw(target);
    
    // 2. Ve Popups (phai ve CUOI CUNG)
    formPopup.draw(target);
    monTapPopup.draw(target);
    deletePopup.draw(target);
}