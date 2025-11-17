// GUI/Popups/CartPopup.h
#pragma once

#include "BasePopup.h"
#include "InputBox.h"
#include "Button.h"
#include "Selector.h"     // De tim HoiVien
#include "TabManager.h"    // De tim San Pham
#include "HoaDon.h"
#include "HoiVien.h"
#include "HangHoa.h"
#include "GoiTap.h"
#include <functional>
#include <vector>
#include <string>

/*
 * (Component con)
 * Mot item trong gio hang (hien thi o Column 3)
 */
struct CartItem {
    std::string id;      // ID cua HangHoa hoac GoiTap
    std::string ten;
    bool isGoiTap;     // De phan biet HangHoa / GoiTap
    int soLuong;
    double donGia;
    
    // Con tro den data goc de hoan tra so luong neu xoa khoi gio
    HangHoa* hhData = nullptr; 
};

/*
 * (Component con)
 * Dinh nghia 2 nut de chon Phuong Thuc Thanh Toan
 */
class PhuongThucSelector {
public:
    sf::Text label;
    Button btnTienMat;
    Button btnChuyenKhoan;
    std::string selected; // "Tien Mat" hoac "Chuyen Khoan"
    sf::Font& font;

    PhuongThucSelector(sf::Font& font);
    void setup(float x, float y);
    void draw(sf::RenderTarget& target);
    void handleEvent(sf::Event event, sf::Vector2i mousePos);
    void update(sf::Vector2i mousePos);
    void setSelected(const std::string& pt);
};


/*
 * Lop CartPopup la giao dien chinh 3 cot de tao va thanh toan hoa don.
 */ 
class CartPopup : public BasePopup {
private:
    // HoaDon* currentHoaDon; // Khong can nua
    std::function<void()> onSuccessCallback; // Goi khi thanh toan thanh cong

    // --- Data tam thoi ---
    std::vector<CartItem> itemsInCart; // Gio hang hien tai (Column 3)
    double tongHoaDon;
    double giamGia;
    double tongThanhToan;
    
    // --- Column 1: Thong Tin ---
    Selector<HoiVien> khachHangSelector;
    sf::Text nhanVienText;
    PhuongThucSelector phuongThucSelector; 

    // --- Column 2: San Pham ---
    TabManager sanPhamTabs;
    InputBox searchSanPham;
    sf::RectangleShape sanPhamListBg; // Nen cho danh sach san pham
    int hoveredSanPhamIndex;
    
    // Data cho danh sach san pham
    MyVector<HangHoa*> dsHangHoa; // Da loc
    MyVector<GoiTap*> dsGoiTap; // Da loc
    
    // --- Column 3: Chi Tiet Hoa Don ---
    sf::RectangleShape cartListBg; // Nen cho gio hang
    int hoveredCartItemIndex; // De xoa
    
    sf::Text tongText;
    sf::Text giamGiaText;
    sf::Text finalTotalText;
    sf::Text errorMessage;
    
    // --- Nut dieu khien ---
    // (Khong co nut Luu)
    Button confirmPayButton; // Nut "Xac nhan Thanh toan"
    Button cancelButton;

    // --- Ham Helper ---
    void loadData(); // Load HoiVien selector
    void loadSanPhamList(); // Loc va load cot 2
    void tinhTong(); // Tinh toan cot 3
    void handleSubmit(); // Xu ly thanh toan
    
    void handleAddHangHoa(HangHoa* hh); // Click cot 2
    void handleAddGoiTap(GoiTap* gt);  // Click cot 2
    void handleRemoveCartItem(int index); // Click cot 3
    
    void drawCol1(sf::RenderTarget& target);
    void drawCol2(sf::RenderTarget& target);
    void drawCol3(sf::RenderTarget& target);

protected:
    void drawContent(sf::RenderTarget& target) override;

public:
    CartPopup(App& app);
    
    /**
     * @brief Hien thi popup de bat dau 1 giao dich moi.
     * @param onSuccess Callback de goi khi thanh toan thanh cong.
     */
    void show(std::function<void()> onSuccess);
    void hide() override;
    
    void handleEvent(sf::Event event, sf::Vector2i mousePos) override;
    void update(sf::Vector2i mousePos) override;
};