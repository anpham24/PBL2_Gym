// GUI/Screens/GoiTapScreen.cpp
#include "GoiTapScreen.h"
#include "Managers/QuanLy.h"
#include "Services/GoiTapService.h"
#include "StringUtils.h"

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

    searchBox.setup("Tim theo Ten hoac Thoi gian (ngay)...", app.getGlobalFont(), false);
    searchBox.setSize(400, 40);
    searchBox.setPosition(contentX, 40);
    
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

void GoiTapScreen::applySearch() {
    std::string searchTerm = searchBox.getString();
    
    // Nếu không có từ khóa → Hiển thị tất cả
    if (searchTerm.empty()) {
        pagination.setup(allGoiTap.size(), 10);
        onPageChange(pagination.getCurrentPage());
        return;
    }
    
    std::cout << "\n🔍 GoiTap - Searching: \"" << searchTerm << "\"" << std::endl;
    
    MyVector<GoiTap*> filteredData;
    
    const MyVector<GoiTap*>& dsGoiTapGoc = app.getQuanLy().getDsGoiTap();
    
    // ✅ 1. Nếu là số → Tìm theo thời gian (chính xác)
    if (StringUtils::isNumber(searchTerm)) {
        int thoiGian = std::stoi(searchTerm);
        std::cout << "   → Searching by Time: " << thoiGian << " days" << std::endl;
        
        for (size_t i = 0; i < dsGoiTapGoc.size(); ++i) {
            GoiTap* gt = dsGoiTapGoc[i];
            if (gt->getThoiGian() == thoiGian) {
                filteredData.push_back(gt);
            }
        }
        
        std::cout << "   ✅ Found " << filteredData.size() << " results" << std::endl;
    } 
    // ✅ 2. Nếu là chữ → Tìm theo tên (linear search)
    else {
        std::cout << "   → Searching by Name..." << std::endl;
        
        for (size_t i = 0; i < dsGoiTapGoc.size(); ++i) {
            GoiTap* gt = dsGoiTapGoc[i];
            
            // ✅ Dùng StringUtils::contains (case-insensitive)
            if (StringUtils::contains(gt->getTenGoi(), searchTerm)) {
                filteredData.push_back(gt);
            }
        }
        
        std::cout << "   ✅ Found " << filteredData.size() << " results" << std::endl;
    }
    
    // ✅ Cập nhật allGoiTap với dữ liệu đã lọc
    allGoiTap.clear();
    for (size_t i = 0; i < filteredData.size(); ++i) {
        allGoiTap.push_back(filteredData[i]);
    }
    
    pagination.setup(allGoiTap.size(), 10);
    onPageChange(1);
}

// --- Ham Logic (DA CAP NHAT) ---
void GoiTapScreen::loadAndDisplayData() {
    allGoiTap.clear();
    
    const MyVector<GoiTap*>& dsGoiTapGoc = app.getQuanLy().getDsGoiTap();
    for (size_t i = 0; i < dsGoiTapGoc.size(); ++i) {
        allGoiTap.push_back(dsGoiTapGoc[i]);
    }
    
    applySearch();
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

    // Xử lý Popups
    if (formPopup.getIsVisible()) { formPopup.handleEvent(event, mousePos); return; }
    if (monTapPopup.getIsVisible()) { monTapPopup.handleEvent(event, mousePos); return; }
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
    
    // Xử lý màn hình chính
    if (! isStaffReadOnly) { 
        themGoiTapButton.handleEvent(event, mousePos);
    }
    goiTapTable.handleEvent(event, mousePos);
    pagination.handleEvent(event, mousePos);
}

void GoiTapScreen::update(sf::Time dt) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(app. getWindow());

    formPopup.update(mousePos);
    monTapPopup.update(mousePos);
    deletePopup.update(mousePos);
    
    if (formPopup.getIsVisible() || monTapPopup.getIsVisible() || deletePopup. getIsVisible()) {
        return; 
    }
    
    searchBox.update(sf::Time::Zero);
    
    if (! isStaffReadOnly) { 
        themGoiTapButton.update(mousePos);
    }
    goiTapTable.update(mousePos);
    pagination.update(mousePos);
}

void GoiTapScreen::draw(sf::RenderTarget& target) {
    searchBox.draw(target);
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