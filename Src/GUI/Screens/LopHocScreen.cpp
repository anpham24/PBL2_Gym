// GUI/Screens/LopHocScreen.cpp
#include "LopHocScreen.h"
#include "Managers/QuanLy.h"
#include "Services/LopHocService.h" // Goi Service
#include "StringUtils.h"

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

    searchBox.setup("Tim theo Ten lop hoac Lich tap (VD: T4-18:00)...", app.getGlobalFont(), false);
    searchBox.setSize(500, 40);
    searchBox.setPosition(contentX, 40);
    
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

void LopHocScreen::applySearch() {
    std::string searchTerm = searchBox.getString();
    
    if (searchTerm.empty()) {
        pagination.setup(allLopHoc.size(), 10);
        onPageChange(pagination.getCurrentPage());
        return;
    }
    
    std::cout << "\n🔍 LopHoc - Searching: \"" << searchTerm << "\"" << std::endl;
    
    MyVector<LopHoc*> filteredData;
    
    const MyVector<LopHoc*>& dsLopHocGoc = app.getQuanLy(). getDsLopHoc();
    
    for (size_t i = 0; i < dsLopHocGoc.size(); ++i) {
        LopHoc* lh = dsLopHocGoc[i];
        
        // ✅ Tìm theo TÊN LỚP (case-insensitive)
        bool matchName = StringUtils::contains(lh->getTenLop(), searchTerm);
        
        // ✅ Tìm theo LỊCH TẬP (VD: "T4-18:00" hoặc chỉ "T4")
        bool matchSchedule = StringUtils::contains(lh->getLichTap(), searchTerm);
        
        if (matchName || matchSchedule) {
            filteredData.push_back(lh);
        }
    }
    
    std::cout << "   ✅ Found " << filteredData.size() << " results" << std::endl;
    
    allLopHoc.clear();
    for (size_t i = 0; i < filteredData.size(); ++i) {
        allLopHoc.push_back(filteredData[i]);
    }
    
    pagination.setup(allLopHoc.size(), 10);
    onPageChange(1);
}

// --- Ham Logic ---
void LopHocScreen::loadAndDisplayData() {
    allLopHoc.clear();
    
    const MyVector<LopHoc*>& dsLopHocGoc = app.getQuanLy().getDsLopHoc();
    for (size_t i = 0; i < dsLopHocGoc.size(); ++i) {
        allLopHoc.push_back(dsLopHocGoc[i]);
    }
    
    // (Sau nay se them logic Tim kiem/Loc o day)
    
    applySearch();
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

    if (formPopup.getIsVisible()) { formPopup.handleEvent(event, mousePos); return; }
    if (deletePopup.getIsVisible()) { deletePopup.handleEvent(event, mousePos); return; }
    
    // ✅ XỬ LÝ SEARCH BOX
    if (auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            if (searchBox.isMouseOver(mousePos)) {
                searchBox.setFocus(true);
            } else {
                searchBox.setFocus(false);
            }
        }
    }
    
    if (searchBox.getFocus()) {
        if (event.getIf<sf::Event::TextEntered>()) {
            searchBox.handleEvent(event);
            applySearch();
        }
        
        if (auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
            searchBox.handleEvent(event);
            if (keyEvent->code == sf::Keyboard::Key::Enter) {
                applySearch();
            }
        }
    }
    
    if (! isStaffReadOnly) { 
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

    searchBox.update(sf::Time::Zero);

    // Update man hinh chinh
    if (!isStaffReadOnly) {
        themLopHocButton.update(mousePos);
    }
    lopHocTable.update(mousePos);
    pagination.update(mousePos);
}

void LopHocScreen::draw(sf::RenderTarget& target) {
    searchBox.draw(target);
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