// GUI/Popups/CartPopup.cpp
#include "CartPopup.h"
#include "HoaDonService.h"
#include "DateTimeUils.h"
#include "QuanLy.h"
#include <iostream>
#include <iomanip> // de format so

// ... (PhuongThucSelector implementation GIU NGUYEN) ...
PhuongThucSelector::PhuongThucSelector(sf::Font& font) 
    : font(font), selected(""), label(font, "P.Thuc TT:", 16), btnTienMat(), btnChuyenKhoan()
{
    label.setFillColor(Config::TextMuted);
    btnTienMat.setup("Tien Mat", font);
    btnChuyenKhoan.setup("Chuyen Khoan", font);
    btnTienMat.setSize(120, 35);
    btnChuyenKhoan.setSize(120, 35);
    btnTienMat.setOnClick([this](){ setSelected("Tien Mat"); });
    btnChuyenKhoan.setOnClick([this](){ setSelected("Chuyen Khoan"); });
}
void PhuongThucSelector::setup(float x, float y) {
    label.setPosition(sf::Vector2f(x, y + 5));
    btnTienMat.setPosition(x + 100, y);
    btnChuyenKhoan.setPosition(x + 230, y);
}
void PhuongThucSelector::draw(sf::RenderTarget& target) {
    target.draw(label);
    btnTienMat.draw(target);
    btnChuyenKhoan.draw(target);
}
void PhuongThucSelector::handleEvent(sf::Event event, sf::Vector2i mousePos) {
    btnTienMat.handleEvent(event, mousePos);
    btnChuyenKhoan.handleEvent(event, mousePos);
}
void PhuongThucSelector::update(sf::Vector2i mousePos) {
    btnTienMat.update(mousePos);
    btnChuyenKhoan.update(mousePos);
}
void PhuongThucSelector::setSelected(const std::string& pt) {
    selected = pt;
    if (pt == "Tien Mat") {
        btnTienMat.setColors(Config::AccentCyan, Config::AccentCyan);
        btnChuyenKhoan.setColors(Config::CardLight, Config::CardDark);
    } else {
        btnTienMat.setColors(Config::CardLight, Config::CardDark);
        btnChuyenKhoan.setColors(Config::AccentCyan, Config::AccentCyan);
    }
}

// ========================================================================
// IMPLEMENTATION cho CartPopup
// ========================================================================

CartPopup::CartPopup(App& app)
    : BasePopup(app, "Tao Hoa Don Moi"), 
      onSuccessCallback(nullptr),
      tongHoaDon(0), giamGia(0), tongThanhToan(0),
      khachHangSelector(app.getGlobalFont(), "Tim khach hang (SDT/Ten)..."),
      sanPhamTabs(app.getGlobalFont()),
      hoveredCartItemIndex(-1),
      phuongThucSelector(app.getGlobalFont()),
      nhanVienText(font, "", 16),
      tongText(font, "", 16),
      giamGiaText(font, "", 16),
      finalTotalText(font, "", 22),
      errorMessage(font, "", 16),
      confirmPayButton(),
      cancelButton()
{
    float popupW = app.getWindow().getSize().x * 0.95f; 
    float popupH = app.getWindow().getSize().y * 0.9f; 
    
    popupPanel.setSize(sf::Vector2f(popupW, popupH));
    popupPanel.setPosition(sf::Vector2f(
        (app.getWindow().getSize().x - popupW) / 2.0f,
        (app.getWindow().getSize().y - popupH) / 2.0f
    ));

    title.setPosition(sf::Vector2f(popupPanel.getPosition().x + 20, popupPanel.getPosition().y + 15));
    closeButton.setPosition(popupPanel.getPosition().x + popupW - 40, popupPanel.getPosition().y + 15);
    
    float pX = popupPanel.getPosition().x;
    float pY = popupPanel.getPosition().y;
    float pTop = pY + 80;
    float pBottom = pY + popupH - 80; 

    // --- Column 1 (25%) ---
    float col1X = pX + 20;
    float col1Width = popupW * 0.25f - 20;
    
    khachHangSelector.setSize(col1Width, 35);
    khachHangSelector.setPosition(col1X, pTop);
    khachHangSelector.setOnSelect([this](HoiVien* hv){
        this->tinhTong(); 
    });
    
    nhanVienText.setFillColor(Config::TextMuted);
    nhanVienText.setPosition(sf::Vector2f(col1X, pTop + 50));
    
    phuongThucSelector.setup(col1X, pTop + 90);

    // --- Column 2 (45% - San Pham) ---
    float col2X = pX + popupW * 0.25f + 10;
    float col2Width = popupW * 0.45f - 20;
    
    sanPhamTabs.setPosition(col2X, pTop);
    sanPhamTabs.addTab("Hang Hoa");
    sanPhamTabs.addTab("Goi Tap");
    
    searchSanPham.setup("Tim san pham...", font, false);
    searchSanPham.setSize(col2Width, 35);
    searchSanPham.setPosition(col2X, pTop + 40);
    
    sanPhamListBg.setPosition(sf::Vector2f(col2X, pTop + 85));
    sanPhamListBg.setSize(sf::Vector2f(col2Width, pBottom - (pTop + 85)));
    sanPhamListBg.setFillColor(Config::CardLight);
    
    // --- Column 3 (30% - Gio Hang) ---
    float col3X = pX + popupW * 0.7f + 10;
    float col3Width = popupW * 0.3f - 30;

    cartListBg.setPosition(sf::Vector2f(col3X, pTop));
    cartListBg.setSize(sf::Vector2f(col3Width, pBottom - (pTop + 150)));
    cartListBg.setFillColor(Config::CardLight);
    
    float totalY = pBottom - 130;
    tongText.setFillColor(Config::TextNormal);
    tongText.setPosition(sf::Vector2f(col3X, totalY));
    
    giamGiaText.setFillColor(Config::AccentCyan);
    giamGiaText.setPosition(sf::Vector2f(col3X, totalY + 30));
    
    finalTotalText.setFillColor(Config::Success);
    finalTotalText.setStyle(sf::Text::Bold);
    finalTotalText.setPosition(sf::Vector2f(col3X, totalY + 60));

    errorMessage.setFillColor(Config::Danger);
    errorMessage.setPosition(sf::Vector2f(pX + 20, pBottom + 20));

    cancelButton.setup("Huy", font);
    cancelButton.setSize(120, 45);
    cancelButton.setPosition(pX + popupW - 300, pBottom + 10);
    cancelButton.setOnClick([this](){ hide(); });

    confirmPayButton.setup("Thanh Toan", font);
    confirmPayButton.setSize(150, 45);
    confirmPayButton.setPosition(pX + popupW - 170, pBottom + 10);
    confirmPayButton.setOnClick([this](){ handleSubmit(); });
}

void CartPopup::show(std::function<void()> onSuccess) {
    onSuccessCallback = onSuccess;
    itemsInCart.clear();
    
    khachHangSelector.setItems(
        app.getQuanLy().getDsHoiVien().getAllValues(), 
        [](HoiVien* hv){ return hv->getHoTen() + " (" + hv->getSDT() + ")"; }
    );
    khachHangSelector.clear();
    searchSanPham.setString("");
    
    if (app.getCurrentAccount() != nullptr && app.getCurrentAccount()->getLinkedStaff() != nullptr) {
        std::string tenNV = app.getCurrentAccount()->getLinkedStaff()->getHoTen();
        nhanVienText.setString("Nhan vien: " + sf::String::fromUtf8(tenNV.begin(), tenNV.end()));
    } else {
        nhanVienText.setString("Nhan vien: ADMIN");
    }
    
    phuongThucSelector.setSelected("Tien Mat");
    
    loadSanPhamList();
    tinhTong();
    BasePopup::show();
}

void CartPopup::hide() {
    BasePopup::hide();
    // Hoan tra so luong ao khi huy (neu can logic nay, hien tai ta tru truc tiep khi thanh toan)
    itemsInCart.clear();
}

void CartPopup::loadSanPhamList() {
    dsHangHoa.clear();
    dsGoiTap.clear();
    std::string term = searchSanPham.getString();
    
    if (sanPhamTabs.getActiveTab() == 0) { // Tab Hang Hoa
        const MyVector<HangHoa*>& ds = app.getQuanLy().getDsHangHoa();
        for (size_t i = 0; i < ds.size(); ++i) {
            HangHoa* hh = ds[i];
            if (hh->getIsActive() && hh->getSoLuongCon() > 0) {
                std::string tenHH = hh->getTenHH();
                if (term.empty() || tenHH.find(term) != std::string::npos) {
                    dsHangHoa.push_back(hh);
                }
            }
        }
    } else { // Tab Goi Tap
        const MyVector<GoiTap*>& ds = app.getQuanLy().getDsGoiTap();
        for (size_t i = 0; i < ds.size(); ++i) {
            GoiTap* gt = ds[i];
            if (gt->getIsActive()) {
                std::string tenGT = gt->getTenGoi();
                if (term.empty() || tenGT.find(term) != std::string::npos) {
                    dsGoiTap.push_back(gt);
                }
            }
        }
    }
}

void CartPopup::tinhTong() {
    tongHoaDon = 0;
    giamGia = 0;
    
    for (const auto& item : itemsInCart) {
        tongHoaDon += item.donGia * item.soLuong;
    }
    
    HoiVien* hv = khachHangSelector.getSelected();
    if (hv != nullptr) {
        // Gia su giam gia 0% cho don gian
        giamGia = 0; 
    }
    
    tongThanhToan = tongHoaDon - giamGia;
    
    tongText.setString("Tong: " + std::to_string((long long)tongHoaDon) + " VND");
    giamGiaText.setString("Giam gia: - " + std::to_string((long long)giamGia) + " VND");
    finalTotalText.setString("Thanh toan: " + std::to_string((long long)tongThanhToan) + " VND");
}

// --- Helpers ---
CartItem* CartPopup::getCartItem(const std::string& id, bool isGoiTap) {
    for (auto& item : itemsInCart) {
        if (item.id == id && item.isGoiTap == isGoiTap) {
            return &item;
        }
    }
    return nullptr;
}

void CartPopup::modifyQuantity(HangHoa* hh, int delta) {
    CartItem* item = getCartItem(hh->getID(), false);
    
    if (item == nullptr) {
        if (delta > 0) { // Them moi
            CartItem newItem;
            newItem.id = hh->getID();
            newItem.ten = hh->getTenHH();
            newItem.donGia = hh->getGia();
            newItem.soLuong = 1;
            newItem.isGoiTap = false;
            newItem.hhData = hh;
            itemsInCart.push_back(newItem);
        }
    } else {
        int newQty = item->soLuong + delta;
        if (newQty > hh->getSoLuongCon()) {
            // Khong du ton kho (logic don gian: khong tang nua)
            // Ban co the hien thong bao loi neu muon
        } else if (newQty <= 0) {
            // Xoa khoi gio
            for (size_t i = 0; i < itemsInCart.size(); ++i) {
                if (itemsInCart[i].id == hh->getID() && !itemsInCart[i].isGoiTap) {
                    itemsInCart.erase(itemsInCart.begin() + i);
                    break;
                }
            }
        } else {
            item->soLuong = newQty;
        }
    }
    tinhTong();
}

void CartPopup::toggleGoiTap(GoiTap* gt) {
    CartItem* item = getCartItem(gt->getID(), true);
    if (item) {
        // Da co -> Xoa
        for (size_t i = 0; i < itemsInCart.size(); ++i) {
            if (itemsInCart[i].id == gt->getID() && itemsInCart[i].isGoiTap) {
                itemsInCart.erase(itemsInCart.begin() + i);
                break;
            }
        }
    } else {
        // Chua co -> Them
        CartItem newItem;
        newItem.id = gt->getID();
        newItem.ten = gt->getTenGoi();
        newItem.donGia = gt->getGia();
        newItem.soLuong = 1; // Goi tap luon la 1
        newItem.isGoiTap = true;
        itemsInCart.push_back(newItem);
    }
    tinhTong();
}

void CartPopup::handleRemoveCartItem(int index) {
    if (index >= 0 && index < itemsInCart.size()) {
        itemsInCart.erase(itemsInCart.begin() + index);
        tinhTong();
    }
}

void CartPopup::handleSubmit() {
    HoiVien* hv = khachHangSelector.getSelected();
    NhanVien* nv = app.getCurrentAccount()->getLinkedStaff();
    
    if (hv == nullptr) { errorMessage.setString("Loi: Chon khach hang."); return; }
    if (itemsInCart.empty()) { errorMessage.setString("Loi: Gio hang trong."); return; }

    std::string now = DateTimeUtils::getCurrentDateTime().substr(0, 10);
    std::string nvID = (nv != nullptr) ? nv->getID() : "ADMIN";
    
    HoaDon* hd = HoaDonService::taoHoaDon(nvID, hv->getID(), now, phuongThucSelector.selected);
    if (hd == nullptr) return;
    
    for (const auto& item : itemsInCart) {
        if (item.isGoiTap) {
            HoaDonService::themGoiTapVaoHoaDon(hd->getID(), item.id, item.soLuong, item.donGia);
        } else {
            // Can kiem tra lai ton kho lan cuoi truoc khi tru
            HoaDonService::themHangHoaVaoHoaDon(hd->getID(), item.id, item.soLuong, item.donGia);
        }
    }
    
    int points = tongThanhToan / Config::MONEY_TO_POINT_RATIO;
    hv->setPoint(hv->getPoint() + points);
    app.getQuanLy().setDirty(true);
    
    if (onSuccessCallback) onSuccessCallback();
    hide();
}

// --- DRAW & EVENT ---

void CartPopup::drawCol2(sf::RenderTarget& target) {
    sanPhamTabs.draw(target);
    searchSanPham.draw(target);
    target.draw(sanPhamListBg);

    sf::FloatRect bounds = sanPhamListBg.getGlobalBounds();
    float pX = bounds.position.x + 10;
    float pY = bounds.position.y + 10;
    float rowHeight = 45; // Tang chieu cao de chua nut
    float width = bounds.size.x - 20;

    sf::Text txt(font, "", 16);
    txt.setFillColor(Config::TextNormal);
    
    // Nut cong/tru (ve bang hinh chu nhat)
    sf::RectangleShape btnRect(sf::Vector2f(30, 30));
    btnRect.setOutlineThickness(1);
    btnRect.setOutlineColor(Config::CardLight);

    if (sanPhamTabs.getActiveTab() == 0) { // Hang Hoa
        for (size_t i = 0; i < dsHangHoa.size(); ++i) {
            float curY = pY + i * rowHeight;
            if (curY + rowHeight > bounds.position.y + bounds.size.y) break;

            HangHoa* hh = dsHangHoa[i];
            CartItem* inCart = getCartItem(hh->getID(), false);
            int qty = inCart ? inCart->soLuong : 0;
            int stock = hh->getSoLuongCon();

            // Ten & Gia
            txt.setString(hh->getTenHH());
            txt.setPosition(sf::Vector2f(pX, curY + 5));
            target.draw(txt);

            std::string priceStr = std::to_string((int)hh->getGia()) + " VND";
            txt.setString(priceStr);
            txt.setPosition(sf::Vector2f(pX, curY + 22));
            txt.setFillColor(Config::TextMuted);
            txt.setCharacterSize(12);
            target.draw(txt);
            txt.setCharacterSize(16); txt.setFillColor(Config::TextNormal); // Reset

            // So luong:  [-]  0/100  [+]
            float ctrlX = pX + width - 110;
            
            // Nut [-]
            btnRect.setPosition(sf::Vector2f(ctrlX, curY));
            btnRect.setFillColor(Config::CardDark);
            target.draw(btnRect);
            txt.setString("-"); txt.setPosition(sf::Vector2f(ctrlX + 10, curY + 2));
            target.draw(txt);

            // So luong text
            std::string qtyStr = std::to_string(qty) + "/" + std::to_string(stock);
            txt.setString(qtyStr);
            txt.setPosition(sf::Vector2f(ctrlX + 40, curY + 5));
            target.draw(txt);

            // Nut [+]
            btnRect.setPosition(sf::Vector2f(ctrlX + 80, curY));
            target.draw(btnRect);
            txt.setString("+"); txt.setPosition(sf::Vector2f(ctrlX + 88, curY + 2));
            target.draw(txt);
        }
    } else { // Goi Tap
        for (size_t i = 0; i < dsGoiTap.size(); ++i) {
            float curY = pY + i * rowHeight;
            if (curY + rowHeight > bounds.position.y + bounds.size.y) break;
            
            GoiTap* gt = dsGoiTap[i];
            bool selected = (getCartItem(gt->getID(), true) != nullptr);

            txt.setString(gt->getTenGoi());
            txt.setPosition(sf::Vector2f(pX, curY + 10));
            target.draw(txt);

            // Nut Them/Xoa
            float btnW = 80;
            btnRect.setSize(sf::Vector2f(btnW, 30));
            btnRect.setPosition(sf::Vector2f(pX + width - btnW, curY + 5));
            btnRect.setFillColor(selected ? Config::Danger : Config::Success);
            target.draw(btnRect);

            txt.setString(selected ? "Xoa" : "Them");
            txt.setPosition(sf::Vector2f(pX + width - btnW + 20, curY + 5));
            target.draw(txt);
        }
    }
}

void CartPopup::handleEvent(sf::Event event, sf::Vector2i mousePos) {
    if (!isVisible) return;
    BasePopup::handleEvent(event, mousePos);
    
    khachHangSelector.handleEvent(event, mousePos);
    phuongThucSelector.handleEvent(event, mousePos);
    if (khachHangSelector.getIsOpen()) return;

    sanPhamTabs.handleEvent(event, mousePos);
    if (event.getIf<sf::Event::TextEntered>()) {
        searchSanPham.handleEvent(event);
        loadSanPhamList();
    }
    
    if (auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            sf::Vector2f mPos(mousePos);
            
            // Xu ly click trong danh sach san pham
            if (sanPhamListBg.getGlobalBounds().contains(mPos)) {
                float listY = sanPhamListBg.getGlobalBounds().position.y + 10;
                float rowHeight = 45;
                int idx = (mPos.y - listY) / rowHeight;
                
                // Cot 2 width
                float width = sanPhamListBg.getSize().x - 20;
                float pX = sanPhamListBg.getPosition().x + 10;

                if (sanPhamTabs.getActiveTab() == 0) { // Hang Hoa
                    if (idx >= 0 && idx < dsHangHoa.size()) {
                        float ctrlX = pX + width - 110;
                        // Check click nut [-]
                        sf::FloatRect minusBtn({ctrlX, listY + idx*rowHeight}, {30, 30});
                        if (minusBtn.contains(mPos)) modifyQuantity(dsHangHoa[idx], -1);
                        
                        // Check click nut [+]
                        sf::FloatRect plusBtn({ctrlX + 80, listY + idx*rowHeight}, {30, 30});
                        if (plusBtn.contains(mPos)) modifyQuantity(dsHangHoa[idx], 1);
                    }
                } else { // Goi Tap
                    if (idx >= 0 && idx < dsGoiTap.size()) {
                        float btnW = 80;
                        sf::FloatRect toggleBtn({pX + width - btnW, listY + idx*rowHeight + 5}, {btnW, 30});
                        if (toggleBtn.contains(mPos)) toggleGoiTap(dsGoiTap[idx]);
                    }
                }
            }
            
            // Xu ly xoa trong gio hang (Cot 3)
             if (cartListBg.getGlobalBounds().contains(mPos)) {
                if (hoveredCartItemIndex != -1) handleRemoveCartItem(hoveredCartItemIndex);
            }
        }
    }
    
    confirmPayButton.handleEvent(event, mousePos);
    cancelButton.handleEvent(event, mousePos);
}

// ... (Cac ham ve cot 1, cot 3, update GIU NGUYEN nhu file cu) ...
// Ban chi can thay the drawCol2 va handleEvent la duoc.
void CartPopup::drawCol1(sf::RenderTarget& target) {
    khachHangSelector.draw(target);
    target.draw(nhanVienText);
    phuongThucSelector.draw(target);
}

void CartPopup::drawCol3(sf::RenderTarget& target) {
    target.draw(cartListBg);
    
    sf::Text itemText(font, "", 14);
    sf::Text priceText(font, "", 14);
    sf::Text deleteText(font, "Xoa", 14);
    deleteText.setFillColor(Config::Danger);

    sf::FloatRect bounds = cartListBg.getGlobalBounds();
    float pX = bounds.position.x + 10;
    float pY = bounds.position.y + 10;
    float rowHeight = 40; 

    for (size_t i = 0; i < itemsInCart.size(); ++i) {
        const auto& item = itemsInCart[i];
        float currentY = pY + i * rowHeight;
        if (currentY > bounds.position.y + bounds.size.y - rowHeight) break;

        if (i == hoveredCartItemIndex) {
            sf::RectangleShape hoverBg(sf::Vector2f(bounds.size.x, rowHeight));
            hoverBg.setPosition(sf::Vector2f(bounds.position.x, currentY - 5));
            hoverBg.setFillColor(Config::CardDark);
            target.draw(hoverBg);
        }
        
        std::string s = std::to_string(i + 1) + ". " + item.ten;
        itemText.setString(sf::String::fromUtf8(s.begin(), s.end()));
        itemText.setPosition(sf::Vector2f(pX, currentY));
        itemText.setFillColor(Config::TextNormal);
        target.draw(itemText);
        
        s = std::to_string(item.soLuong) + " x " + std::to_string((long long)item.donGia);
        priceText.setString(sf::String::fromUtf8(s.begin(), s.end()));
        priceText.setPosition(sf::Vector2f(pX + 15, currentY + 18));
        priceText.setFillColor(Config::TextMuted);
        target.draw(priceText);
        
        deleteText.setPosition(sf::Vector2f(pX + bounds.size.x - 40, currentY + 5));
        target.draw(deleteText);
    }
    
    target.draw(tongText);
    target.draw(giamGiaText);
    target.draw(finalTotalText);
}

void CartPopup::update(sf::Vector2i mousePos) {
    if (!isVisible) return;
    
    khachHangSelector.update(mousePos);
    phuongThucSelector.update(mousePos);
    confirmPayButton.update(mousePos);
    cancelButton.update(mousePos);
    
    // Cap nhat hovered index cho cart items
    hoveredCartItemIndex = -1;
    if (cartListBg.getGlobalBounds().contains(sf::Vector2f(mousePos))) {
        float listY = cartListBg.getGlobalBounds().position.y + 10;
        float rowHeight = 40;
        int idx = (mousePos.y - listY) / rowHeight;
        if (idx >= 0 && idx < itemsInCart.size()) {
            hoveredCartItemIndex = idx;
        }
    }
}

void CartPopup::drawContent(sf::RenderTarget& target) {
    drawCol1(target);
    drawCol2(target);
    drawCol3(target);
    
    confirmPayButton.draw(target);
    cancelButton.draw(target);
    target.draw(errorMessage);
}