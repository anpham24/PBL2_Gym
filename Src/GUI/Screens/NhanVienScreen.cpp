// GUI/Screens/NhanVienScreen.cpp
#include "NhanVienScreen.h"
#include "Managers/QuanLy.h"
#include "Services/NhanVienService.h" // Goi Service

// --- Constructor (DA CAP NHAT QUYEN) ---
NhanVienScreen::NhanVienScreen(App& app) 
    : BaseScreen(app), 
      nhanVienTable(app.getGlobalFont()), 
      pagination(app.getGlobalFont()),
      formPopup(app),                 // Khoi tao popup
      detailPopup(app),               // Khoi tao popup
      deletePopup(app)                // Khoi tao popup
{
    // --- (MOI) KIEM TRA QUYEN ---
    isStaffReadOnly = (app.getCurrentAccount()->getAccountType() == AccountType::STAFF);

    float contentX = 250.0f; 
    
    // --- Nut Them ---
    themNhanVienButton.setup("Them Nhan Vien Moi", app.getGlobalFont());
    themNhanVienButton.setSize(200, 40);
    themNhanVienButton.setPosition(app.getWindow().getSize().x - 230, 40);
    themNhanVienButton.setOnClick([this]() {
        this->formPopup.show(nullptr, [this]() {
            this->loadAndDisplayData(); // Callback load lai data
        });
    });

    // --- Bang Du Lieu ---
    nhanVienTable.setPosition(contentX, 100);
    
    // Cac cot luon hien thi
    nhanVienTable.addColumn("ID", 100, [](const NhanVien* nv) { return nv->getID(); });
    nhanVienTable.addColumn("Ten Nhan Vien", 250, [](const NhanVien* nv) { return nv->getHoTen(); });
    nhanVienTable.addColumn("SDT", 150, [](const NhanVien* nv) { return nv->getSDT(); });
    nhanVienTable.addColumn("Trang Thai", 120, [](const NhanVien* nv) { 
        return nv->getIsActive() ? "Dang lam" : "Nghi viec"; 
    });
    
    // Cot "Chi Tiet" (Nut "Xem" - Luon hien thi)
    nhanVienTable.addAction("Xem", [this](NhanVien* nv) {
        this->detailPopup.show(nv); // Mo popup chi tiet
    }, Config::AccentCyan, 80);
    
    // --- (MOI) CHI ADMIN MOI THAY COT SUA/XOA ---
    if (!isStaffReadOnly) {
        // Cot "Sua"
        nhanVienTable.addEditAction([this](NhanVien* nv) {
            this->formPopup.show(nv, [this]() {
                this->loadAndDisplayData();
            });
        });

        // Cot "Xoa"
        nhanVienTable.addDeleteAction([this](NhanVien* nv) {
            auto onConfirm = [this, nv]() {
                NhanVienService::xoaNhanVien(nv->getID()); // GOI SERVICE
                this->loadAndDisplayData();
            };
            
            // Kiem tra rang buoc
            std::string warning = "";
            if (!nv->getDsHopDong().empty() || !nv->getDsHoaDon().empty()) {
                warning = "Nhan vien nay da xu ly Hop Dong hoac Hoa Don!";
            }
            this->deletePopup.show(nv->getHoTen(), onConfirm, warning);
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
void NhanVienScreen::loadAndDisplayData() {
    allNhanVien.clear();
    
    const MyVector<NhanVien*>& dsNhanVienGoc = app.getQuanLy().getDsNhanVien();
    for (size_t i = 0; i < dsNhanVienGoc.size(); ++i) {
        allNhanVien.push_back(dsNhanVienGoc[i]);
    }
    
    // (Sau nay se them logic Tim kiem/Loc o day)
    
    pagination.setup(allNhanVien.size(), 10);
    onPageChange(pagination.getCurrentPage());
}

void NhanVienScreen::onPageChange(int newPage) {
    pagedNhanVien.clear();
    int startIndex = (newPage - 1) * 10;
    int endIndex = std::min(startIndex + 10, (int)allNhanVien.size());
    
    for (int i = startIndex; i < endIndex; ++i) {
        pagedNhanVien.push_back(allNhanVien.at(i));
    }
    
    nhanVienTable.setData(pagedNhanVien);
}

// --- Vong lap chinh (DA CAP NHAT QUYEN) ---
void NhanVienScreen::handleEvent(sf::Event event) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(app.getWindow());

    // Xu ly Popups truoc
    if (formPopup.getIsVisible()) { formPopup.handleEvent(event, mousePos); return; }
    if (detailPopup.getIsVisible()) { detailPopup.handleEvent(event, mousePos); return; }
    if (deletePopup.getIsVisible()) { deletePopup.handleEvent(event, mousePos); return; }
    
    // Xu ly man hinh chinh
    if (!isStaffReadOnly) { // Staff khong the an nut "Them"
        themNhanVienButton.handleEvent(event, mousePos);
    }
    nhanVienTable.handleEvent(event, mousePos);
    pagination.handleEvent(event, mousePos);
}

void NhanVienScreen::update(sf::Time dt) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(app.getWindow());

    // Luon update cac popup
    formPopup.update(mousePos);
    detailPopup.update(mousePos);
    deletePopup.update(mousePos);
    
    if (formPopup.getIsVisible() || detailPopup.getIsVisible() || deletePopup.getIsVisible()) {
        return; 
    }

    // Update man hinh chinh
    if (!isStaffReadOnly) {
        themNhanVienButton.update(mousePos);
    }
    nhanVienTable.update(mousePos);
    pagination.update(mousePos);
}

void NhanVienScreen::draw(sf::RenderTarget& target) {
    // 1. Ve man hinh chinh
    if (!isStaffReadOnly) { // Staff khong thay nut "Them"
        themNhanVienButton.draw(target);
    }
    nhanVienTable.draw(target);
    pagination.draw(target);
    
    // 2. Ve Popups (phai ve CUOI CUNG)
    formPopup.draw(target);
    detailPopup.draw(target);
    deletePopup.draw(target);
}