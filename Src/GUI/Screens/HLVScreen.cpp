// GUI/Screens/HLVScreen.cpp
#include "HLVScreen.h"
#include "QuanLy.h"
#include "HLVService.h" // Goi Service
#include "StringUtils.h"

// --- Constructor (DA CAP NHAT QUYEN) ---
HLVScreen::HLVScreen(App& app) 
    : BaseScreen(app), 
      hlvTable(app.getGlobalFont()), 
      pagination(app.getGlobalFont()),
      formPopup(app),                 // Khoi tao popup
      detailPopup(app),               // Khoi tao popup
      deletePopup(app)                // Khoi tao popup
{
    // --- (MOI) KIEM TRA QUYEN ---
    isStaffReadOnly = (app.getCurrentAccount()->getAccountType() == AccountType::STAFF);

    float contentX = 250.0f;
    
    searchBox.setup("Tim theo ID, Ten hoac Chuyen mon...", app.getGlobalFont(), false);
    searchBox.setSize(450, 40);
    searchBox.setPosition(contentX, 40);
    
    // --- Nut Them ---
    themHLVButton.setup("Them HLV Moi", app.getGlobalFont());
    themHLVButton.setSize(200, 40);
    themHLVButton.setPosition(app.getWindow().getSize().x - 230, 40);
    themHLVButton.setOnClick([this]() {
        this->formPopup.show(nullptr, [this]() {
            this->loadAndDisplayData(); // Callback load lai data
        });
    });

    // --- Bang Du Lieu ---
    hlvTable.setPosition(contentX, 100);
    
    // === SỬA LẠI CÁC CỘT Ở ĐÂY ===
    // Cot ID: Lay ID
    hlvTable.addColumn("ID", 100, [](const HLV* hlv) { 
        return hlv->getID(); 
    });

    // Cot Ten: Lay Ho Ten (Truoc day ban co the dang lay ID)
    hlvTable.addColumn("Ten HLV", 250, [](const HLV* hlv) { 
        return hlv->getHoTen(); 
    });

    // Cot SDT: Lay SDT (Truoc day ban co the dang lay Ten)
    hlvTable.addColumn("SDT", 150, [](const HLV* hlv) { 
        return hlv->getSDT(); 
    });

    // Cot Trang Thai
    hlvTable.addColumn("Trang Thai", 120, [](const HLV* hlv) { 
        return hlv->getIsActive() ? "Dang lam" : "Nghi viec"; 
    });
    // ==============================
    
    // Cot "Chi Tiet" (Nut "Xem" - Luon hien thi)
    hlvTable.addAction("Xem", [this](HLV* hlv) {
        this->detailPopup.show(hlv); // Mo popup chi tiet
    }, Config::AccentCyan, 80);
    
    // --- (MOI) CHI ADMIN MOI THAY COT SUA/XOA ---
    if (!isStaffReadOnly) {
        // Cot "Sua"
        hlvTable.addEditAction([this](HLV* hlv) {
            this->formPopup.show(hlv, [this]() {
                this->loadAndDisplayData();
            });
        });

        // Cot "Xoa"
        hlvTable.addDeleteAction([this](HLV* hlv) {
            auto onConfirm = [this, hlv]() {
                HLVService::xoaHLV(hlv->getID()); // GOI SERVICE
                this->loadAndDisplayData();
            };
            
            // Kiem tra rang buoc
            std::string warning = "";
            if (!hlv->getDsLopHoc().empty() || !hlv->getDsLogTapPT().empty()) {
                warning = "HLV nay dang phu trach Lop Hoc hoac Log Tap PT!";
            }
            this->deletePopup.show(hlv->getHoTen(), onConfirm, warning);
        });
    }

    // --- Phan Trang ---
    pagination.setPosition(contentX, 600);
    pagination.setOnPageChange([this](int newPage) {
        this->onPageChange(newPage);
    });
    
    loadAndDisplayData();
}

void HLVScreen::applySearch() {
    std::string searchTerm = searchBox.getString();
    
    // Reload từ QuanLy
    allHLV.clear();
    const MyVector<HLV*>& dsHLVGoc = app.getQuanLy().getDsHLV();
    for (size_t i = 0; i < dsHLVGoc.size(); ++i) {
        allHLV.push_back(dsHLVGoc[i]);
    }
    
    if (searchTerm.empty()) {
        std::cout << "🔍 Search cleared, showing all " << allHLV.size() << " HLV" << std::endl;
        pagination.setup(allHLV.size(), 10);
        onPageChange(pagination.getCurrentPage());
        return;
    }
    
    std::cout << "\n🔍 HLV - Searching: \"" << searchTerm << "\"" << std::endl;
    
    MyVector<HLV*> filteredData;
    
    // ✅ 1.  Nếu toàn số → Tìm theo ID (prefix match)
    if (StringUtils::isNumber(searchTerm)) {
        std::cout << "   → Searching by ID (Prefix Match)..." << std::endl;
        
        for (size_t i = 0; i < dsHLVGoc.size(); ++i) {
            HLV* hlv = dsHLVGoc[i];
            
            if (hlv->getID().substr(0, searchTerm.length()) == searchTerm) {
                filteredData.push_back(hlv);
            }
        }
        
        std::cout << "   ✅ Found " << filteredData.size() << " results" << std::endl;
    } 
    // ✅ 2. Nếu chữ → Tìm theo TÊN hoặc CHUYÊN MÔN
    else {
        std::cout << "   → Searching by Name/Chuyen Mon..." << std::endl;
        
        // Kiểm tra input hợp lệ
        bool hasInvalidChar = false;
        for (size_t i = 0; i < searchTerm.length(); ++i) {
            char c = searchTerm[i];
            if (! ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ')) {
                hasInvalidChar = true;
                break;
            }
        }
        
        if (hasInvalidChar) {
            std::cout << "   ⚠️  Contains invalid chars → No search" << std::endl;
        } else {
            // Tìm theo TÊN hoặc CHUYÊN MÔN (Linear Search)
            for (size_t i = 0; i < dsHLVGoc.size(); ++i) {
                HLV* hlv = dsHLVGoc[i];
                
                bool matchName = StringUtils::contains(hlv->getHoTen(), searchTerm);
                bool matchChuyenMon = StringUtils::contains(hlv->getChuyenMon(), searchTerm);
                
                if (matchName || matchChuyenMon) {
                    filteredData.push_back(hlv);
                }
            }
            
            std::cout << "   ✅ Found " << filteredData.size() << " results" << std::endl;
        }
    }
    
    allHLV.clear();
    for (size_t i = 0; i < filteredData. size(); ++i) {
        allHLV.push_back(filteredData[i]);
    }
    
    pagination. setup(allHLV.size(), 10);
    onPageChange(1);
}

// --- Ham Logic ---
void HLVScreen::loadAndDisplayData() {
    allHLV.clear();
    
    const MyVector<HLV*>& dsHLVGoc = app.getQuanLy(). getDsHLV();
    for (size_t i = 0; i < dsHLVGoc.size(); ++i) {
        allHLV.push_back(dsHLVGoc[i]);
    }
    
    applySearch(); // ✅ ÁP DỤNG TÌM KIẾM
}

void HLVScreen::onPageChange(int newPage) {
    pagedHLV.clear();
    int startIndex = (newPage - 1) * 10;
    int endIndex = std::min(startIndex + 10, (int)allHLV.size());
    
    for (int i = startIndex; i < endIndex; ++i) {
        pagedHLV.push_back(allHLV.at(i));
    }
    
    hlvTable.setData(pagedHLV);
}

// --- Vong lap chinh (DA CAP NHAT QUYEN) ---
void HLVScreen::handleEvent(sf::Event event) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(app.getWindow());

    if (formPopup.getIsVisible()) { formPopup.handleEvent(event, mousePos); return; }
    if (detailPopup.getIsVisible()) { detailPopup.handleEvent(event, mousePos); return; }
    if (deletePopup.getIsVisible()) { deletePopup.handleEvent(event, mousePos); return; }
    
    // ✅ XỬ LÝ SEARCH BOX
    if (auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            searchBox.setFocus(searchBox.isMouseOver(mousePos));
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
        themHLVButton.handleEvent(event, mousePos);
    }
    hlvTable. handleEvent(event, mousePos);
    pagination.handleEvent(event, mousePos);
}

void HLVScreen::update(sf::Time dt) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(app.getWindow());

    formPopup.update(mousePos);
    detailPopup.update(mousePos);
    deletePopup.update(mousePos);
    
    if (formPopup.getIsVisible() || detailPopup.getIsVisible() || deletePopup.getIsVisible()) {
        return; 
    }

    searchBox. update(sf::Time::Zero); // ✅ THÊM
    
    if (!isStaffReadOnly) {
        themHLVButton.update(mousePos);
    }
    hlvTable.update(mousePos);
    pagination.update(mousePos);
}

void HLVScreen::draw(sf::RenderTarget& target) {
    searchBox.draw(target); // ✅ THÊM
    
    if (!isStaffReadOnly) {
        themHLVButton.draw(target);
    }
    hlvTable.draw(target);
    pagination.draw(target);
    
    formPopup.draw(target);
    detailPopup.draw(target);
    deletePopup. draw(target);
}