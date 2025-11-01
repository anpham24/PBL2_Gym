#ifndef MEMBERSCREEN_H
#define MEMBERSCREEN_H

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

// Bao gồm các file cần thiết
#include "BaseScreen.h"
#include "../Manager/DataManager.h" // Dùng interface
#include "../UI/ModernInputBox.h"
#include "../UI/ModernButton.h"
#include "../UI/DropdownMenu.h"

// Sử dụng namespace
using namespace sf;
using namespace std;

class MemberScreen : public BaseScreen {
private:
    Font& m_font;
    DataManager& m_dataManager; // Sử dụng interface DataManager

    // ----- UI Chung của Màn hình -----
    Text m_title;
    unique_ptr<ModernInputBox> m_searchBox;
    unique_ptr<ModernButton> m_addButton;
    unique_ptr<ModernButton> m_prevPageButton, m_nextPageButton;
    Text m_pageInfoText;
    Text m_messageText; // Tin nhắn (ví dụ: "Thêm thành công")

    // ----- Trạng thái Dữ liệu -----
    vector<Member> m_searchResults; // Dữ liệu đã lọc
    int m_currentPage;
    int m_itemsPerPage;
    int m_totalPages;

    // ----- Trạng thái Popup -----
    bool m_showAddPopup;
    RectangleShape m_popupOverlay; // Lớp phủ mờ
    RectangleShape m_popupBackground;
    Text m_popupTitle;
    
    // UI Của Popup (2 cột)
    vector<unique_ptr<ModernInputBox>> m_popupInputs; // [0]=Name, [1]=Phone, [2]=Email
    unique_ptr<DropdownMenu> m_popupTypeDropdown;
    unique_ptr<DropdownMenu> m_popupDurationDropdown;
    unique_ptr<ModernButton> m_popupSaveButton;
    unique_ptr<ModernButton> m_popupCancelButton;

    // ----- Các hàm nội bộ (Private) -----
    
    // Khởi tạo
    void setupUI();
    void setupPopupUI();

    // Quản lý dữ liệu
    void refreshData(); // Tải lại dữ liệu từ DataManager
    void updatePagination();
    vector<Member> getCurrentPageData() const;

    // Xử lý Popup
    void openPopup();
    void closePopup();
    void handlePopupEvents(Event& event, const Vector2f& mousePos);
    void saveNewMember();
    
    // Vẽ
    void drawMainUI(RenderWindow& window);
    void drawPopup(RenderWindow& window);
    void drawTable(RenderWindow& window);
    
    // Tiện ích
    void setMessage(const string& msg, Color color);
    
public:
    // Constructor (Sửa lỗi SFML 3: phải nhận font)
    MemberScreen(Font& font, DataManager& dataManager);
    virtual ~MemberScreen() {}

    // Các hàm kế thừa từ BaseScreen
    void handleEvent(Event& event, const Vector2f& mousePos) override;
    void update(float dt) override;
    void draw(RenderWindow& window) override;
};

#endif // MEMBERSCREEN_H