// GUI/Screens/HoiVienScreen.cpp
#include "HoiVienScreen.h"
#include "QuanLy.h"
#include "HoiVienService.h" // De goi xoaHoiVien
#include "StringUtils.h"
#include "SearchService.h"

HoiVienScreen::HoiVienScreen(App& app) 
    : BaseScreen(app), 
      hoiVienTable(app.getGlobalFont()), 
      pagination(app.getGlobalFont()),
      formPopup(app),
      cartPopup(app),           // ✅ Khởi tạo TRƯỚC
      datHLVPopup(app),         // ✅ Khởi tạo TRƯỚC
      detailPopup(app, cartPopup, datHLVPopup), // ✅ Truyền đủ tham số
      deletePopup(app)
{
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

    searchBox.setup("Tim theo ID hoac Ten...", app.getGlobalFont(), false);
    searchBox.setSize(400, 40);
    searchBox.setPosition(contentX, 40); // Phía trên table

    // --- Bang Du Lieu ---
    hoiVienTable.setPosition(contentX, 100);
    
    // Cac cot luon hien thi
    hoiVienTable.addColumn("ID", 100, [](const HoiVien* hv) { return hv->getID(); });
    hoiVienTable.addColumn("Ten", 250, [](const HoiVien* hv){ return hv->getHoTen(); });
    hoiVienTable.addColumn("SDT", 150, [](const HoiVien* hv){ return hv->getSDT(); });
    hoiVienTable.addColumn("Trang Thai", 120, [](const HoiVien* hv){ 
        return hv->getIsActive() ? "Hoat dong" : "Het han"; 
    });
    hoiVienTable.addColumn("Rank", 100, [](const HoiVien* hv){ return hv->getRank(); });

    // Cot "Xem Chi Tiet" (luon hien thi)
    hoiVienTable.addViewAction([this](HoiVien* hv){
        // Mo popup "Xem"
        this->detailPopup.show(hv);
    });

    // --- (MOI) CHI ADMIN MOI THAY COT SUA/XOA ---
    // if (!isStaffReadOnly) {
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
    // }

    // --- Phan Trang ---
    pagination.setPosition(contentX, 600); // Dat duoi bang
    pagination.setOnPageChange([this](int newPage) {
        this->onPageChange(newPage);
    });
    
    // --- Load data lan dau ---
    loadAndDisplayData();
}

void HoiVienScreen::applySearch() {
    std::string searchTerm = searchBox. getString();
    
    // ✅ LUÔN reload từ QuanLy trước
    allHoiVien.clear();
    MyVector<HoiVien*> allValues = app. getQuanLy(). getDsHoiVien(). getAllValues();
    for (size_t i = 0; i < allValues.size(); ++i) {
        allHoiVien.push_back(allValues[i]);
    }
    
    if (searchTerm.empty()) {
        std::cout << "🔍 Search cleared, showing all " << allHoiVien. size() << " HoiVien" << std::endl;
        pagination.setup(allHoiVien. size(), 10);
        onPageChange(pagination.getCurrentPage());
        return;
    }
    
    std::cout << "\n🔍 HoiVien - Searching: \"" << searchTerm << "\"" << std::endl;
    
    MyVector<HoiVien*> filteredData;
    
    // ============================================================
    // ✅ KIỂM TRA: Input có phải toàn số không?  → Tìm theo ID
    // ============================================================
    if (StringUtils::isNumber(searchTerm)) {
        std::cout << "   → Searching by ID (Prefix Match)..." << std::endl;
        
        // Duyệt tất cả HV, tìm ID bắt đầu bằng searchTerm
        for (size_t i = 0; i < allHoiVien. size(); ++i) {
            HoiVien* hv = allHoiVien[i];
            
            // Kiểm tra ID có bắt đầu bằng searchTerm không
            if (hv->getID().substr(0, searchTerm.length()) == searchTerm) {
                filteredData. push_back(hv);
            }
        }
        
        std::cout << "   ✅ Found " << filteredData.size() << " results" << std::endl;
    } 
    // ============================================================
    // ✅ Ngược lại: Tìm theo TÊN (dùng Trie)
    // ============================================================
    else {
        std::cout << "   → Searching by Name (Trie)..." << std::endl;
        
        // Kiểm tra input có chứa số hoặc ký tự đặc biệt không
        bool hasInvalidChar = false;
        
        for (size_t i = 0; i < searchTerm.length(); ++i) {
            char c = searchTerm[i];
            
            // Chỉ chấp nhận a-z, A-Z, dấu cách
            if (! ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ')) {
                hasInvalidChar = true;
                break;
            }
        }
        
        // Nếu có ký tự không hợp lệ → Không tìm
        if (hasInvalidChar) {
            std::cout << "   ⚠️  Contains invalid chars → No search" << std::endl;
        } 
        // Nếu hợp lệ → Tìm bằng Trie
        else {
            MyVector<HoiVien*> results = SearchService::searchHoiVien(searchTerm);
            
            for (size_t i = 0; i < results.size(); ++i) {
                filteredData.push_back(results[i]);
            }
            
            std::cout << "   ✅ Found " << filteredData.size() << " results" << std::endl;
        }
    }
    
    // Cập nhật danh sách hiển thị
    allHoiVien. clear();
    for (size_t i = 0; i < filteredData.size(); ++i) {
        allHoiVien.push_back(filteredData[i]);
    }
    
    pagination.setup(allHoiVien.size(), 10);
    onPageChange(1);
}

void HoiVienScreen::loadAndDisplayData() {
    allHoiVien.clear();
    
    MyVector<HoiVien*> allValues = app.getQuanLy().getDsHoiVien(). getAllValues();
    for (size_t i = 0; i < allValues.size(); ++i) {
        allHoiVien.push_back(allValues[i]);
    }
    
    // ✅ ÁP DỤNG TÌM KIẾM
    applySearch();
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

    // ✅ XỬ LÝ POPUPS - RETURN NGAY SAU KHI XỬ LÝ
    if (datHLVPopup.getIsVisible()) {
        datHLVPopup.handleEvent(event, mousePos);
        return; // ✅ QUAN TRỌNG: Ngăn xử lý tiếp
    }
    
    if (cartPopup.getIsVisible()) {
        cartPopup.handleEvent(event, mousePos);
        return;
    }
    
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
            applySearch(); // ✅ Tìm kiếm mỗi khi gõ
        }
        
        if (auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
            searchBox.handleEvent(event);
            if (keyEvent->code == sf::Keyboard::Key::Enter) {
                applySearch();
            }
        }
    }
    
    // Xử lý màn hình chính
    if (!isStaffReadOnly) {
        themHoiVienButton.handleEvent(event, mousePos);
    }
    hoiVienTable.handleEvent(event, mousePos);
    pagination.handleEvent(event, mousePos);
}

void HoiVienScreen::update(sf::Time dt) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(app.getWindow());

    formPopup.update(mousePos);
    detailPopup.update(mousePos);
    deletePopup.update(mousePos);
    cartPopup.update(mousePos);      // ✅ THÊM
    datHLVPopup.update(mousePos);    // ✅ THÊM
    
    if (formPopup.getIsVisible() || detailPopup.getIsVisible() || 
        deletePopup.getIsVisible() || cartPopup.getIsVisible() || 
        datHLVPopup.getIsVisible()) {
        return;
    }

    searchBox.update(sf::Time::Zero);

    if (!isStaffReadOnly) {
        themHoiVienButton.update(mousePos);
    }
    hoiVienTable.update(mousePos);
    pagination.update(mousePos);
}

void HoiVienScreen::draw(sf::RenderTarget& target) {
    searchBox.draw(target); 

    if (!isStaffReadOnly) {
        themHoiVienButton.draw(target);
    }
    hoiVienTable.draw(target);
    pagination.draw(target);
    
    // ✅ Vẽ popups
    formPopup.draw(target);
    detailPopup.draw(target);
    deletePopup.draw(target);
    cartPopup.draw(target);      // ✅ THÊM
    datHLVPopup.draw(target);    // ✅ THÊM
}