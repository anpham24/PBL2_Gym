// GUI/Screens/HoiVienScreen.cpp
#include "HoiVienScreen.h"
#include "QuanLy.h"
#include "HoiVienService.h" // De goi xoaHoiVien

HoiVienScreen::HoiVienScreen(App& app) 
    : BaseScreen(app), 
      hoiVienTable(app.getGlobalFont()), 
      pagination(app.getGlobalFont()),
      formPopup(app),       // Khoi tao popup
      detailPopup(app),     // Khoi tao popup
      deletePopup(app)      // Khoi tao popup
{
    // --- (MOI) KIEM TRA QUYEN ---
    // Neu la Staff, dat co ReadOnly
    isStaffReadOnly = (app.getCurrentAccount()->getAccountType() == AccountType::STAFF);

    float contentX = 250; // Vi tri bat dau (sau Sidebar)
    
    // --- Nut Them ---
    themHoiVienButton.setup("Them Hoi Vien Moi", app.getGlobalFont());
    themHoiVienButton.setSize(200, 40);
    themHoiVienButton.setPosition(app.getWindow().getSize().x - 230, 40);
    themHoiVienButton.setOnClick([this]() {
        // Goi popup them moi, voi callback de load lai data khi thanh cong
        this->formPopup.show(nullptr, [this]() {
            this->loadAndDisplayData();
        });
    });

    // --- Bang Du Lieu ---
    hoiVienTable.setPosition(contentX, 100);
    
    // Cac cot luon hien thi
    hoiVienTable.addColumn("ID", 100, [](const HoiVien* hv) { return hv->getID(); });
    hoiVienTable.addColumn("Ten", 250, [](const HoiVien* hv){ return hv->getHoTen(); });
    hoiVienTable.addColumn("SDT", 150, [](const HoiVien* hv){ return hv->getSDT(); });
    hoiVienTable.addColumn("Trang Thai", 120, [](const HoiVien* hv){ 
        return hv->getIsActive() ? "Hoat dong" : "Het han"; 
    });
    hoiVienTable.addColumn("Rank", 100, [](const HoiVien* hv){ 
        // (Ban se tu them logic getRank() sau)
        return "Bronze"; 
    });

    // Cot "Xem Chi Tiet" (luon hien thi)
    hoiVienTable.addViewAction([this](HoiVien* hv){
        // Mo popup "Xem"
        this->detailPopup.show(hv);
    });

    // --- (MOI) CHI ADMIN MOI THAY COT SUA/XOA ---
    if (!isStaffReadOnly) {
        // Cot "Sua"
        hoiVienTable.addEditAction([this](HoiVien* hv){
            // Goi popup sua, voi callback
            this->formPopup.show(hv, [this]() {
                this->loadAndDisplayData();
            });
        });

        // Cot "Xoa"
        hoiVienTable.addDeleteAction([this](HoiVien* hv){
            // Dinh nghia ham callback
            auto onConfirm = [this, hv]() {
                HoiVienService::xoaHoiVien(hv->getID()); // GOI SERVICE
                this->loadAndDisplayData(); // Load lai sau khi xoa
            };
            
            // Kiem tra rang buoc truoc khi hien popup
            std::string warning = "";
            if (hv->getIsActive() || !hv->getDsHopDong().empty()) {
                warning = "Canh bao: Hoi vien nay van con dang hoat dong!";
            }
            
            this->deletePopup.show(hv->getHoTen(), onConfirm, warning);
        });
    }

    // --- Phan Trang ---
    pagination.setPosition(contentX, 600); // Dat duoi bang
    pagination.setOnPageChange([this](int newPage) {
        this->onPageChange(newPage);
    });
    
    // --- Load data lan dau ---
    loadAndDisplayData();
}

void HoiVienScreen::loadAndDisplayData() {
    allHoiVien.clear();
    
    // Lay data tu QuanLy (HoiVien dung HashTable nen can getAllValues)
    MyVector<HoiVien*> allValues = app.getQuanLy().getDsHoiVien().getAllValues();
    for (size_t i = 0; i < allValues.size(); ++i) {
        allHoiVien.push_back(allValues[i]);
    }
    
    // (Sau nay se them logic Tim kiem/Loc o day)
    
    // Thiet lap pagination
    pagination.setup(allHoiVien.size(), 10); // 10 muc moi trang
    
    // Hien thi trang hien tai
    onPageChange(pagination.getCurrentPage());
}

void HoiVienScreen::onPageChange(int newPage) {
    pagedHoiVien.clear(); // Xoa data trang cu
    
    int startIndex = (newPage - 1) * 10;
    int endIndex = std::min(startIndex + 10, (int)allHoiVien.size());
    
    for (int i = startIndex; i < endIndex; ++i) {
        pagedHoiVien.push_back(allHoiVien.at(i));
    }
    
    // Nap data vao bang
    hoiVienTable.setData(pagedHoiVien);
}

// --- Vong lap chinh (Event, Update, Draw) ---
void HoiVienScreen::handleEvent(sf::Event event) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(app.getWindow());

    // --- Xu ly Popups truoc (de chan click) ---
    // Neu co popup dang mo, chi popup do duoc nhan su kien
    if (formPopup.getIsVisible()) {
        formPopup.handleEvent(event, mousePos);
        return;
    }
    if (detailPopup.getIsVisible()) {
        detailPopup.handleEvent(event, mousePos);
        return;
    }
    if (deletePopup.getIsVisible()) {
        deletePopup.handleEvent(event, mousePos);
        return;
    }
    
    // --- Neu khong co popup, xu ly man hinh chinh ---
    if (!isStaffReadOnly) { // Staff khong the an nut "Them"
        themHoiVienButton.handleEvent(event, mousePos);
    }
    hoiVienTable.handleEvent(event, mousePos);
    pagination.handleEvent(event, mousePos);
}

void HoiVienScreen::update(sf::Time dt) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(app.getWindow());

    // Luon update cac popup (vi chung co the dang an/hien)
    formPopup.update(mousePos);
    detailPopup.update(mousePos);
    deletePopup.update(mousePos);
    
    // Neu co popup dang mo, khong update man hinh chinh
    if (formPopup.getIsVisible() || detailPopup.getIsVisible() || deletePopup.getIsVisible()) {
        return; 
    }

    // Update man hinh chinh
    if (!isStaffReadOnly) {
        themHoiVienButton.update(mousePos);
    }
    hoiVienTable.update(mousePos);
    pagination.update(mousePos);
}

void HoiVienScreen::draw(sf::RenderTarget& target) {
    // 1. Ve man hinh chinh
    if (!isStaffReadOnly) { // Staff khong thay nut "Them"
        themHoiVienButton.draw(target);
    }
    hoiVienTable.draw(target);
    pagination.draw(target);
    
    // 2. Ve Popups (phai ve CUOI CUNG de no noi len tren)
    formPopup.draw(target);
    detailPopup.draw(target);
    deletePopup.draw(target);
}