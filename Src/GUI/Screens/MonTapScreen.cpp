// GUI/Screens/MonTapScreen.cpp
#include "MonTapScreen.h"
#include "QuanLy.h"
#include "MonTapService.h" // Goi Service
#include "StringUtils.h"

// --- Constructor ---
MonTapScreen::MonTapScreen(App& app) 
    : BaseScreen(app), 
      monTapTable(app.getGlobalFont()), 
      pagination(app.getGlobalFont()),
      formPopup(app),                 // Khoi tao popup
      deletePopup(app)                // Khoi tao popup
{
    // --- (MOI) KIEM TRA QUYEN ---
    isStaffReadOnly = (app.getCurrentAccount()->getAccountType() == AccountType::STAFF);

    float contentX = 250.0f; // Vi tri bat dau (sau Sidebar)

    searchBox.setup("Tim theo Ten mon.. .", app.getGlobalFont(), false);
    searchBox.setSize(350, 40);
    searchBox.setPosition(contentX, 40);
    
    // --- Nut Them ---
    themMonTapButton.setup("Them Mon Tap Moi", app.getGlobalFont());
    themMonTapButton.setSize(200, 40);
    themMonTapButton.setPosition(app.getWindow().getSize().x - 230, 40);
    themMonTapButton.setOnClick([this]() {
        // Goi popup them moi, voi callback de load lai data khi thanh cong
        this->formPopup.show(nullptr, [this]() {
            this->loadAndDisplayData();
        });
    });

    // --- Bang Du Lieu ---
    monTapTable.setPosition(contentX, 100);
    
    // Cac cot luon hien thi
    monTapTable.addColumn("ID", 120, [](const MonTap* mt) { return mt->getID(); });
    monTapTable.addColumn("Ten Mon", 300, [](const MonTap* mt) { return mt->getTenMon(); });
    
    // --- (MOI) CHI ADMIN MOI THAY COT SUA/XOA ---
    if (!isStaffReadOnly) {
        // Cot "Sua"
        monTapTable.addEditAction([this](MonTap* mt) {
            // Goi popup sua, voi callback
            this->formPopup.show(mt, [this]() {
                this->loadAndDisplayData();
            });
        });

        // Cot "Xoa"
        monTapTable.addDeleteAction([this](MonTap* mt) {
            auto onConfirm = [this, mt]() {
                MonTapService::xoaMonTap(mt->getID()); // GOI SERVICE
                this->loadAndDisplayData(); // Load lai sau khi xoa
            };
            
            // Kiem tra rang buoc (neu Mon Tap co Lop Hoc hoac Goi Tap)
            std::string warning = "";
            if (!mt->getDsLopHoc().empty() || !mt->getDsGoiTap().empty()) {
                warning = "Mon tap nay dang duoc su dung trong Goi Tap hoac Lop Hoc!";
            }
            this->deletePopup.show(mt->getTenMon(), onConfirm, warning);
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

MonTapScreen::~MonTapScreen() {}

void MonTapScreen::applySearch() {
    std::string searchTerm = searchBox.getString();
    
    // Reload từ QuanLy
    allMonTap.clear();
    const MyVector<MonTap*>& dsMonTapGoc = app.getQuanLy(). getDsMonTap();
    for (size_t i = 0; i < dsMonTapGoc.size(); ++i) {
        allMonTap. push_back(dsMonTapGoc[i]);
    }
    
    if (searchTerm.empty()) {
        pagination.setup(allMonTap. size(), 10);
        onPageChange(pagination.getCurrentPage());
        return;
    }
    
    std::cout << "\n🔍 MonTap - Searching: \"" << searchTerm << "\"" << std::endl;
    
    MyVector<MonTap*> filteredData;
    
    // Kiểm tra input hợp lệ (chỉ chữ cái + dấu cách)
    bool hasInvalidChar = false;
    for (size_t i = 0; i < searchTerm.length(); ++i) {
        char c = searchTerm[i];
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ')) {
            hasInvalidChar = true;
            break;
        }
    }
    
    if (hasInvalidChar) {
        std::cout << "   ⚠️  Contains invalid chars → No search" << std::endl;
    } else {
        // ✅ Tìm theo TÊN (Linear Search)
        for (size_t i = 0; i < dsMonTapGoc.size(); ++i) {
            MonTap* mt = dsMonTapGoc[i];
            
            if (StringUtils::contains(mt->getTenMon(), searchTerm)) {
                filteredData.push_back(mt);
            }
        }
        
        std::cout << "   ✅ Found " << filteredData.size() << " results" << std::endl;
    }
    
    allMonTap.clear();
    for (size_t i = 0; i < filteredData. size(); ++i) {
        allMonTap.push_back(filteredData[i]);
    }
    
    pagination. setup(allMonTap.size(), 10);
    onPageChange(1);
}

// --- Ham Logic ---
void MonTapScreen::loadAndDisplayData() {
    allMonTap.clear();
    
    const MyVector<MonTap*>& dsMonTapGoc = app.getQuanLy().getDsMonTap();
    for (size_t i = 0; i < dsMonTapGoc.size(); ++i) {
        allMonTap.push_back(dsMonTapGoc[i]);
    }
    
    applySearch(); // ✅ ÁP DỤNG TÌM KIẾM
}

void MonTapScreen::onPageChange(int newPage) {
    pagedMonTap.clear();
    int startIndex = (newPage - 1) * 10;
    int endIndex = std::min(startIndex + 10, (int)allMonTap.size());
    
    for (int i = startIndex; i < endIndex; ++i) {
        pagedMonTap.push_back(allMonTap.at(i));
    }
    
    monTapTable.setData(pagedMonTap);
}

// --- Vong lap chinh (Event, Update, Draw) ---
void MonTapScreen::handleEvent(sf::Event event) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(app.getWindow());

    if (formPopup.getIsVisible()) { formPopup.handleEvent(event, mousePos); return; }
    if (deletePopup.getIsVisible()) { deletePopup. handleEvent(event, mousePos); return; }
    
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
        themMonTapButton.handleEvent(event, mousePos);
    }
    monTapTable.handleEvent(event, mousePos);
    pagination.handleEvent(event, mousePos);
}

void MonTapScreen::update(sf::Time dt) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(app.getWindow());

    formPopup.update(mousePos);
    deletePopup. update(mousePos);
    
    if (formPopup.getIsVisible() || deletePopup.getIsVisible()) {
        return; 
    }

    searchBox.update(sf::Time::Zero); // ✅ THÊM
    
    if (! isStaffReadOnly) {
        themMonTapButton.update(mousePos);
    }
    monTapTable.update(mousePos);
    pagination.update(mousePos);
}

void MonTapScreen::draw(sf::RenderTarget& target) {
    searchBox.draw(target); // ✅ THÊM
    
    if (!isStaffReadOnly) {
        themMonTapButton.draw(target);
    }
    monTapTable.draw(target);
    pagination.draw(target);
    
    formPopup.draw(target);
    deletePopup.draw(target);
}

