// GUI/Popups/CartPopup.h
#pragma once

#include "BasePopup.h"
#include "InputBox.h"
#include "Button.h"
#include "Selector.h"     
#include "TabManager.h"    
#include "HoaDon.h"
#include "HoiVien.h"
#include "HangHoa.h"
#include "GoiTap.h"
#include "HoiVienSelector.h"
#include <functional>
#include <vector>
#include <string>

struct CartItem {
    std::string id;      
    std::string ten;
    bool isGoiTap;     
    int soLuong;
    double donGia;
    
    HangHoa* hhData = nullptr; 
};

class PhuongThucSelector {
public:
    sf::Text label;
    Button btnTienMat;
    Button btnChuyenKhoan;
    std::string selected; 
    sf::Font& font;

    PhuongThucSelector(sf::Font& font);
    void setup(float x, float y);
    void draw(sf::RenderTarget& target);
    void handleEvent(sf::Event event, sf::Vector2i mousePos);
    void update(sf::Vector2i mousePos);
    void setSelected(const std::string& pt);
}; 

class CartPopup : public BasePopup {
private:
    std::function<void()> onSuccessCallback; 

    // --- Data ---
    std::vector<CartItem> itemsInCart; 
    double tongHoaDon;
    double giamGia;
    double tongThanhToan;
    
    // --- Column 1 ---
    HoiVienSelector khachHangSelector;
    sf::Text nhanVienText;
    PhuongThucSelector phuongThucSelector; 

    // --- Column 2 ---
    TabManager sanPhamTabs;
    InputBox searchSanPham;
    sf::RectangleShape sanPhamListBg; 
    // (Bo bien hoveredSanPhamIndex vi gio ta xu ly button cu the)
    
    MyVector<HangHoa*> dsHangHoa; 
    MyVector<GoiTap*> dsGoiTap; 
    
    // --- Column 3 ---
    sf::RectangleShape cartListBg; 
    int hoveredCartItemIndex; 
    
    sf::Text tongText;
    sf::Text giamGiaText;
    sf::Text finalTotalText;
    sf::Text errorMessage;
    
    // --- Nut dieu khien ---
    Button confirmPayButton; 
    Button cancelButton;

    // --- Ham Helper ---
    void loadData(); 
    void loadSanPhamList(); 
    void tinhTong(); 
    void handleSubmit(); 
    
    // Helper de tim item trong gio hang
    CartItem* getCartItem(const std::string& id, bool isGoiTap);

    // Cac ham thao tac gio hang
    void modifyQuantity(HangHoa* hh, int delta); // Tang/Giam so luong
    void toggleGoiTap(GoiTap* gt); // Them/Xoa goi tap
    void handleRemoveCartItem(int index); 
    
    void drawCol1(sf::RenderTarget& target);
    void drawCol2(sf::RenderTarget& target); // <--- Viet lai ham nay
    void drawCol3(sf::RenderTarget& target);

protected:
    void drawContent(sf::RenderTarget& target) override;

public:
    CartPopup(App& app);

    void show(HoiVien* hv, std::function<void()> onSuccess);

    void show(HoiVien* hv, const std::string& goiID, std::function<void()> onSuccess);
    
    void show(std::function<void()> onSuccess);
    void hide() override;
    
    void handleEvent(sf::Event event, sf::Vector2i mousePos) override; // <--- Viet lai ham nay
    void update(sf::Vector2i mousePos) override;
};