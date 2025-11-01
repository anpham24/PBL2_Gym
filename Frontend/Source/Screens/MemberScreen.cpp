#include "MemberScreen.h"
#include <iostream> // Để debug (có thể xóa)

// Sử dụng namespace
using namespace sf;
using namespace std;

// Sửa lỗi SFML 3: Khởi tạo các biến sf::Text trong constructor
MemberScreen::MemberScreen(Font& font, DataManager& dataManager)
    : m_font(font), 
      m_dataManager(dataManager), 
      m_currentPage(0), 
      m_itemsPerPage(8), // Hiển thị 8 mục mỗi trang
      m_totalPages(1), 
      m_showAddPopup(false),
      m_title(m_font),
      m_pageInfoText(m_font),
      m_messageText(m_font),
      m_popupTitle(m_font)
{
    setupUI();
    setupPopupUI();
    refreshData(); // Tải dữ liệu lần đầu
}

void MemberScreen::setupUI() {
    m_title.setString("Quan Ly Thanh Vien");
    m_title.setCharacterSize(32);
    m_title.setFillColor(Color(70, 130, 180));
    m_title.setStyle(Text::Bold);
    m_title.setPosition({300, 100}); // Vị trí bên phải Sidebar

    m_searchBox = make_unique<ModernInputBox>(m_font, "", "Tim kiem (ten, sdt, ma)...", 750, 105, 450, 50);
    m_addButton = make_unique<ModernButton>(m_font, "THEM MOI", 1220, 105, 150, 50);
    
    m_prevPageButton = make_unique<ModernButton>(m_font, "< TRUOC", 300, 850, 150, 40);
    m_nextPageButton = make_unique<ModernButton>(m_font, "SAU >", 1220, 850, 150, 40);

    m_pageInfoText.setCharacterSize(16);
    m_pageInfoText.setFillColor(Color(100, 100, 100));
    
    m_messageText.setCharacterSize(16);
    m_messageText.setPosition({300, 820});
}

void MemberScreen::setupPopupUI() {
    // Lớp phủ
    m_popupOverlay.setSize({1400, 900});
    m_popupOverlay.setFillColor(Color(0, 0, 0, 150));
    
    // Nền popup (lấy từ file gộp)
    m_popupBackground.setSize({600, 400});
    m_popupBackground.setFillColor(Color(255, 255, 255));
    m_popupBackground.setOutlineColor(Color(70, 130, 180));
    m_popupBackground.setOutlineThickness(3);
    m_popupBackground.setPosition({(1400 - 600) / 2.f, (900 - 400) / 2.f}); // Canh giữa

    // Tiêu đề popup
    m_popupTitle.setString("Them Thanh Vien Moi");
    m_popupTitle.setCharacterSize(28);
    m_popupTitle.setFillColor(Color(70, 130, 180));
    m_popupTitle.setStyle(Text::Bold);
    m_popupTitle.setPosition({m_popupBackground.getPosition().x + 20, m_popupBackground.getPosition().y + 20});

    // Vị trí các control (2 cột)
    float popupX = m_popupBackground.getPosition().x;
    float popupY = m_popupBackground.getPosition().y;
    float col1X = popupX + 30;
    float col2X = popupX + 320;
    float inputWidth = 250;
    
    // Hàng 1
    m_popupInputs.push_back(make_unique<ModernInputBox>(m_font, "Ho va ten*", "Nhap ho ten...", col1X, popupY + 100, inputWidth, 50));
    m_popupInputs.push_back(make_unique<ModernInputBox>(m_font, "So dien thoai*", "Nhap so dien thoai...", col2X, popupY + 100, inputWidth, 50));
    
    // Hàng 2
    m_popupInputs.push_back(make_unique<ModernInputBox>(m_font, "Email", "Nhap email...", col1X, popupY + 190, inputWidth, 50));
    m_popupTypeDropdown = make_unique<DropdownMenu>(m_font, "Loai the", 
        vector<string>{"Basic", "Premium", "VIP"}, col2X, popupY + 190, inputWidth, 50);
        
    // Hàng 3
    m_popupDurationDropdown = make_unique<DropdownMenu>(m_font, "Thoi han", 
        vector<string>{"1 thang", "3 thang", "6 thang", "12 thang"}, col1X, popupY + 280, inputWidth, 50);

    // Nút
    m_popupSaveButton = make_unique<ModernButton>(m_font, "LUU", col1X + 50, popupY + 340, 190, 50);
    m_popupCancelButton = make_unique<ModernButton>(m_font, "HUY", col2X, popupY + 340, 190, 50);
}

void MemberScreen::refreshData() {
    // Lấy dữ liệu từ DataManager (Mock hoặc Thật)
    m_searchResults = m_dataManager.searchMembers(m_searchBox->getText());
    updatePagination();
    setMessage("",Color(0, 150, 0)); // Xóa tin nhắn
}

void MemberScreen::updatePagination() {
    m_totalPages = (m_searchResults.size() + m_itemsPerPage - 1) / m_itemsPerPage;
    if (m_totalPages == 0) m_totalPages = 1;
    if (m_currentPage >= m_totalPages) m_currentPage = m_totalPages - 1;
    if (m_currentPage < 0) m_currentPage = 0;
    
    m_pageInfoText.setString("Trang " + to_string(m_currentPage + 1) + " / " + to_string(m_totalPages));
    // Sửa lỗi SFML 3: dùng .size.x
    m_pageInfoText.setPosition({700 + 250/2.f - m_pageInfoText.getLocalBounds().size.x / 2.f, 860});
}

vector<Member> MemberScreen::getCurrentPageData() const {
    int startIdx = m_currentPage * m_itemsPerPage;
    if (startIdx < 0 || startIdx >= m_searchResults.size()) {
        return {}; // Trả về vector rỗng nếu không có dữ liệu
    }
    int endIdx = min(startIdx + m_itemsPerPage, (int)m_searchResults.size());
    
    vector<Member> pageData;
    for (int i = startIdx; i < endIdx; i++) {
        pageData.push_back(m_searchResults[i]);
    }
    return pageData;
}

void MemberScreen::handleEvent(Event& event, const Vector2f& mousePos) {
    // Ưu tiên xử lý popup nếu nó đang mở
    if (m_showAddPopup) {
        handlePopupEvents(event, mousePos);
        return; // Ngăn không cho click "xuyên" qua popup
    }

    // Xử lý các control của màn hình chính
    m_searchBox->handleEvent(event);
    m_addButton->handleEvent(event, mousePos);
    m_prevPageButton->handleEvent(event, mousePos);
    m_nextPageButton->handleEvent(event, mousePos);

    if (m_addButton->isClicked(event, mousePos)) {
        openPopup();
    }
    
    if (m_prevPageButton->isClicked(event, mousePos) && m_currentPage > 0) {
        m_currentPage--;
        updatePagination();
    }
    
    if (m_nextPageButton->isClicked(event, mousePos) && m_currentPage < m_totalPages - 1) {
        m_currentPage++;
        updatePagination();
    }
    
    // Sửa lỗi SFML 3: Dùng getIf
    if (auto* keyEvent = event.getIf<Event::KeyPressed>()) {
        if (keyEvent->code == Keyboard::Key::Enter && m_searchBox->getIsActive()) {
            refreshData();
        }
    }
}

void MemberScreen::handlePopupEvents(Event& event, const Vector2f& mousePos) {
    // Xử lý các input
    for (auto& input : m_popupInputs) {
        input->handleEvent(event);
    }
    m_popupTypeDropdown->handleEvent(event, mousePos);
    m_popupDurationDropdown->handleEvent(event, mousePos);
    
    // Xử lý các nút
    m_popupSaveButton->handleEvent(event, mousePos);
    m_popupCancelButton->handleEvent(event, mousePos);
    
    if (m_popupSaveButton->isClicked(event, mousePos)) {
        saveNewMember();
    }
    
    if (m_popupCancelButton->isClicked(event, mousePos)) {
        closePopup();
    }
}

void MemberScreen::openPopup() {
    m_showAddPopup = true;
    // Reset và set active
    for (auto& input : m_popupInputs) input->clear();
    m_popupTypeDropdown->setSelectedIndex(0);
    m_popupDurationDropdown->setSelectedIndex(0);
    m_popupInputs[0]->setActive(true);
}

void MemberScreen::closePopup() {
    m_showAddPopup = false;
}

void MemberScreen::saveNewMember() {
    string name = m_popupInputs[0]->getText();
    string phone = m_popupInputs[1]->getText();
    string email = m_popupInputs[2]->getText();
    
    if (name.empty() || phone.empty()) {
        setMessage("Vui long dien Ho Ten va So Dien Thoai!", Color::Red);
        // Không đóng popup
        return;
    }
    
    MembershipType type = static_cast<MembershipType>(m_popupTypeDropdown->getSelectedIndex());
    int durationIndex = m_popupDurationDropdown->getSelectedIndex();
    int months = (durationIndex == 0 ? 1 : (durationIndex == 1 ? 3 : (durationIndex == 2 ? 6 : 12)));
    
    m_dataManager.addMember(name, phone, email, type, months);
    
    // Xong, làm mới dữ liệu và đóng popup
    refreshData();
    closePopup();
    setMessage("Them thanh vien thanh cong!", Color(0, 150, 0));
}

void MemberScreen::setMessage(const string& msg, Color color) {
    m_messageText.setString(msg);
    m_messageText.setFillColor(color);
}

void MemberScreen::update(float dt) {
    if (m_showAddPopup) {
        for (auto& input : m_popupInputs) input->update(dt);
    } else {
        m_searchBox->update(dt);
    }
}

void MemberScreen::draw(RenderWindow& window) {
    drawMainUI(window);
    
    if (m_showAddPopup) {
        drawPopup(window);
    }
}

void MemberScreen::drawMainUI(RenderWindow& window) {
    window.draw(m_title);
    m_searchBox->draw(window);
    m_addButton->draw(window);
    m_prevPageButton->draw(window);
    m_nextPageButton->draw(window);
    window.draw(m_pageInfoText);
    window.draw(m_messageText);

    drawTable(window);
}

// Dán code này vào file MemberScreen.cpp, thay thế hàm drawTable cũ

void MemberScreen::drawTable(RenderWindow& window) {
    // Vẽ Bảng
    RectangleShape tableHeader({1070, 40});
    tableHeader.setPosition({300, 160});
    tableHeader.setFillColor(Color(70, 130, 180));
    window.draw(tableHeader);
    
    vector<string> headers = {"Ma", "Ho Ten", "SDT", "Email", "Loai The", "Ngay Het Han", "Trang Thai"};
    vector<float> positions = {310, 380, 550, 700, 850, 980, 1100};
    
    // Sửa lỗi SFML 3: Phải tạo biến Text
    Text headerText(m_font);
    headerText.setCharacterSize(16);
    headerText.setFillColor(Color::White);
    headerText.setStyle(Text::Bold);

    for (size_t i = 0; i < headers.size(); i++) {
        headerText.setString(headers[i]);
        headerText.setPosition({positions[i], 170});
        window.draw(headerText);
    }

    // Lấy dữ liệu
    vector<Member> pageData = getCurrentPageData();
    float yPos = 210;
    
    // Biến Text tái sử dụng để vẽ nội dung
    Text cellText(m_font);
    cellText.setCharacterSize(14);
    cellText.setFillColor(Color(50, 50, 50));

    // SỬA LỖI 1: Thay đổi vòng lặp để lấy chỉ số 'i'
    for (size_t i = 0; i < pageData.size(); ++i) {
        const auto& m = pageData[i]; // Lấy đối tượng member
        
        RectangleShape row({1070, 50});
        row.setPosition({300, yPos});
        
        // SỬA LỖI 1: Dùng 'i % 2' thay vì '&m % 2'
        row.setFillColor(i % 2 == 0 ? Color(250, 250, 250) : Color::White);
        window.draw(row);
        
        // Helper lambda cho gọn
        auto drawCell = [&](const string& s, float x, Color c = Color(50, 50, 50)) {
            cellText.setString(s);
            
            // SỬA LỖI 2: Dùng {x, y} cho setPosition (SFML 3)
            cellText.setPosition({x, yPos + 18}); 
            
            cellText.setFillColor(c);
            window.draw(cellText);
        };
        
        drawCell(m.id, positions[0]);
        drawCell(m.name, positions[1]);
        drawCell(m.phone, positions[2]);
        drawCell(m.email, positions[3]);
        drawCell(m_dataManager.getMembershipTypeName(m.type), positions[4]);
        drawCell(m.endDate, positions[5]);
        drawCell(m.isActive ? "Hoat dong" : "Het han", positions[6], m.isActive ? Color(0, 150, 0) : Color::Red);
        
        yPos += 50;
    }
}

void MemberScreen::drawPopup(RenderWindow& window) {
    window.draw(m_popupOverlay);
    window.draw(m_popupBackground);
    window.draw(m_popupTitle);
    
    for (auto& input : m_popupInputs) {
        input->draw(window);
    }
    m_popupTypeDropdown->draw(window);
    m_popupDurationDropdown->draw(window);
    
    m_popupSaveButton->draw(window);
    m_popupCancelButton->draw(window);
}