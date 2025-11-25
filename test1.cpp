// GUI/Popups/CartPopup.cpp
#include "CartPopup.h"
#include "HoaDonService.h"
#include "DateTimeUils.h"
#include "QuanLy.h"
#include <algorithm>
#include <iostream>

// ========================================================================
// IMPLEMENTATION cho PhuongThucSelector (Component con)
// ========================================================================
PhuongThucSelector::PhuongThucSelector(sf::Font& font) 
    : font(font), selected(""), label(font, "P.Thuc TT:", 16), btnTienMat(), btnChuyenKhoan()
{
    // label.setFont(font);
    // label.setString("P.Thuc TT:");
    // label.setCharacterSize(16);
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
      khachHangSelector(app.getGlobalFont(), "Tim khach hang..."),
      sanPhamTabs(app.getGlobalFont()),
      hoveredSanPhamIndex(-1),
      hoveredCartItemIndex(-1),
      phuongThucSelector(app.getGlobalFont()),
      nhanVienText(font, "", 16),
      tongText(font, "", 16),
      giamGiaText(font, "", 16),
      finalTotalText(font, "", 22), // (Lấy size 22 từ Dòng 161)
      errorMessage(font, "", 16)
{
    // Mo rong popup ra toi da
    float popupW = app.getWindow().getSize().x * 0.9f; // 90% chieu rong
    float popupH = app.getWindow().getSize().y * 0.9f; // 90% chieu cao
    popupPanel.setSize(sf::Vector2f(popupW, popupH));
    popupPanel.setPosition(sf::Vector2f(
        (app.getWindow().getSize().x - popupW) / 2.0f,
        (app.getWindow().getSize().y - popupH) / 2.0f
    ));
    // Dat lai vi tri Title va Nut [X]
    title.setPosition(sf::Vector2f(popupPanel.getPosition().x + 20, popupPanel.getPosition().y + 15));
    closeButton.setPosition(popupPanel.getPosition().x + popupW - 40, popupPanel.getPosition().y + 15);
    
    // Dinh nghia layout 3 cot
    float pX = popupPanel.getPosition().x;
    float pY = popupPanel.getPosition().y;
    float pTop = pY + 80; // Vi tri bat dau duoi Title
    float pBottom = pY + popupH - 80; // Vi tri cho cac nut duoi cung

    // --- Column 1 (30% width) ---
    float col1X = pX + 20;
    float col1Width = popupW * 0.3f - 30;
    
    khachHangSelector.setSize(col1Width, 35);
    khachHangSelector.setPosition(col1X, pTop);
    khachHangSelector.setOnSelect([this](HoiVien* hv){
        this->tinhTong(); // Tinh lai giam gia khi chon HV
    });
    
    // nhanVienText.setFont(font);
    // nhanVienText.setCharacterSize(16);
    nhanVienText.setFillColor(Config::TextMuted);
    nhanVienText.setPosition(sf::Vector2f(col1X, pTop + 50));
    
    phuongThucSelector.setup(col1X, pTop + 90);

    // --- Column 2 (40% width) ---
    float col2X = pX + popupW * 0.3f;
    float col2Width = popupW * 0.4f - 20;
    
    sanPhamTabs.setPosition(col2X, pTop);
    sanPhamTabs.addTab("Hang Hoa");
    sanPhamTabs.addTab("Goi Tap");
    
    searchSanPham.setup("Tim san pham...", font, false);
    searchSanPham.setSize(col2Width, 35);
    searchSanPham.setPosition(col2X, pTop + 40);
    
    sanPhamListBg.setPosition(sf::Vector2f(col2X, pTop + 85));
    sanPhamListBg.setSize(sf::Vector2f(col2Width, pBottom - (pTop + 85)));
    sanPhamListBg.setFillColor(Config::CardLight);
    
    // --- Column 3 (30% width) ---
    float col3X = pX + popupW * 0.7f + 10;
    float col3Width = popupW * 0.3f - 30;

    cartListBg.setPosition(sf::Vector2f(col3X, pTop));
    cartListBg.setSize(sf::Vector2f(col3Width, pBottom - (pTop + 100)));
    cartListBg.setFillColor(Config::CardLight);
    
    // Setup cac text Tong tien
    float totalY = pBottom - 80;
    // tongText.setFont(font);
    // tongText.setCharacterSize(16);
    tongText.setFillColor(Config::TextNormal);
    tongText.setPosition(sf::Vector2f(col3X, totalY));
    
    // giamGiaText.setFont(font);
    // giamGiaText.setCharacterSize(16);
    giamGiaText.setFillColor(Config::AccentCyan);
    giamGiaText.setPosition(sf::Vector2f(col3X, totalY + 30));
    
    // finalTotalText.setFont(font);
    // finalTotalText.setCharacterSize(22);
    finalTotalText.setFillColor(Config::Success);
    finalTotalText.setStyle(sf::Text::Bold);
    finalTotalText.setPosition(sf::Vector2f(col3X, totalY + 60));

    // --- Nut dieu khien (duoi cung) ---
    // errorMessage.setFont(font);
    // errorMessage.setCharacterSize(16);
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
    
    // Load danh sach khach hang (HoiVien) vao selector
    khachHangSelector.setItems(
        app.getQuanLy().getDsHoiVien().getAllValues(), // Lay tu HashTable
        [](HoiVien* hv){ return hv->getHoTen() + " (" + hv->getSDT() + ")"; }
    );
    
    // Reset cac truong
    khachHangSelector.clear();
    searchSanPham.setString("");
    
    // Set Nhan Vien hien tai
    if (app.getCurrentAccount() != nullptr && app.getCurrentAccount()->getLinkedStaff() != nullptr) {
        std::string tenNV = app.getCurrentAccount()->getLinkedStaff()->getHoTen();
        nhanVienText.setString("Nhan vien: " + sf::String::fromUtf8(tenNV.begin(), tenNV.end()));
    } else {
        nhanVienText.setString("Nhan vien: ADMIN (Khong lien ket)");
    }
    
    phuongThucSelector.setSelected("Tien Mat"); // Mac dinh
    
    loadSanPhamList(); // Load danh sach san pham cho Tab Hang Hoa
    tinhTong();
    BasePopup::show();
}

void CartPopup::hide() {
    BasePopup::hide();
    // Tra lai so luong hang hoa neu Huy gio hang
    for (const auto& item : itemsInCart) {
        if (item.hhData != nullptr) {
            item.hhData->setSoLuongCon(item.hhData->getSoLuongCon() + item.soLuong);
        }
    }
    itemsInCart.clear();
}

void CartPopup::loadSanPhamList() {
    dsHangHoa.clear();
    dsGoiTap.clear();
    
    std::string term = searchSanPham.getString();
    // Convert term sang lowercase (nếu cần)
    std::transform(term.begin(), term.end(), term.begin(), ::tolower);
    
    if (sanPhamTabs.getActiveTab() == 0) { // Tab Hang Hoa
        const MyVector<HangHoa*>& ds = app.getQuanLy().getDsHangHoa();
        for (size_t i = 0; i < ds.size(); ++i) {
            HangHoa* hh = ds[i];
            if (hh->getIsActive() && hh->getSoLuongCon() > 0) {
                std::string tenHH = hh->getTenHH();
                std::transform(tenHH.begin(), tenHH.end(), tenHH.begin(), ::tolower);
                
                if (term.empty() || tenHH.find(term) != std::string::npos) {
                    dsHangHoa.push_back(hh);
                }
            }
        }
    } else { // ✅ Tab Goi Tap - FIX
        const MyVector<GoiTap*>& ds = app.getQuanLy().getDsGoiTap();
        for (size_t i = 0; i < ds.size(); ++i) {
            GoiTap* gt = ds[i];
            // ✅ SỬA: Kiểm tra isActive
            if (gt->getIsActive()) {
                std::string tenGT = gt->getTenGoi();
                std::transform(tenGT.begin(), tenGT.end(), tenGT.begin(), ::tolower);
                
                if (term.empty() || tenGT.find(term) != std::string::npos) {
                    dsGoiTap.push_back(gt);
                }
            }
        }
    }
    
    // ✅ DEBUG: In ra console
    std::cout << "📊 Loaded " << dsHangHoa.size() << " Hang Hoa, " 
              << dsGoiTap.size() << " Goi Tap" << std::endl;
}

void CartPopup::tinhTong() {
    tongHoaDon = 0;
    giamGia = 0;
    
    for (const auto& item : itemsInCart) {
        tongHoaDon += item.donGia * item.soLuong;
    }
    
    HoiVien* hv = khachHangSelector.getSelected();
    if (hv != nullptr) {
        // (Ban can them logic getRank() vao HoiVien.h/.cpp)
        // Config::Rank rank = hv->getRank(); 
        // giamGia = tongHoaDon * Config::RANK_DISCOUNT.at(rank);
        
        // Gia lap giam gia 5%
        giamGia = tongHoaDon * 0.05;
    }
    
    tongThanhToan = tongHoaDon - giamGia;
    
    // Cap nhat UI (Column 3)
    tongText.setString("Tong: " + std::to_string((int)tongHoaDon) + " VND");
    giamGiaText.setString("Giam gia: - " + std::to_string((int)giamGia) + " VND");
    finalTotalText.setString("Thanh toan: " + std::to_string((int)tongThanhToan) + " VND");
}

void CartPopup::handleSubmit() {
    HoiVien* hv = khachHangSelector.getSelected();
    NhanVien* nv = app.getCurrentAccount()->getLinkedStaff();
    
    if (hv == nullptr) {
        errorMessage.setString("Loi: Vui long chon mot khach hang.");
        return;
    }
    if (nv == nullptr && app.getCurrentAccount()->getAccountType() != AccountType::ADMIN) {
        errorMessage.setString("Loi: Tai khoan Staff khong lien ket voi Nhan Vien.");
        return;
    }
    if (itemsInCart.empty()) {
        errorMessage.setString("Loi: Gio hang trong.");
        return;
    }

    // --- Tat ca deu hop le: TAO HOA DON ---
    std::string now = DateTimeUtils::getCurrentDateTime().substr(0, 10); // "DD/MM/YYYY"
    
    // Neu la Admin, khong co NhanVien lien ket -> truyen nullptr
    std::string nvID = (nv != nullptr) ? nv->getID() : "ADMIN_ID"; // (Ban can 1 ID mac dinh cho Admin)
    
    HoaDon* hd = HoaDonService::taoHoaDon(nvID, hv->getID(), now, phuongThucSelector.selected);
    
    if (hd == nullptr) {
        errorMessage.setString("Loi he thong: Khong the tao hoa don.");
        return;
    }
    
    // Them item vao HoaDon
    for (const auto& item : itemsInCart) {
        if (item.isGoiTap) {
            HoaDonService::themGoiTapVaoHoaDon(hd->getID(), item.id, item.soLuong, item.donGia);
        } else {
            HoaDonService::themHangHoaVaoHoaDon(hd->getID(), item.id, item.soLuong, item.donGia);
            // (Luu y: Service themHangHoa... da tu dong tru ton kho)
        }
    }
    
    // --- THANH TOAN NGAY ---
    hd->setDaThanhToan(true);
    
    // (Cong diem cho HoiVien)
    int points = tongThanhToan / Config::MONEY_TO_POINT_RATIO;
    hv->setPoint(hv->getPoint() + points);
    
    app.getQuanLy().setDirty(true);
    if (onSuccessCallback) onSuccessCallback();
    hide();
}

void CartPopup::handleEvent(sf::Event event, sf::Vector2i mousePos) {
    if (!isVisible) return;
    BasePopup::handleEvent(event, mousePos);

    // --- Column 1 ---
    khachHangSelector.handleEvent(event, mousePos);
    phuongThucSelector.handleEvent(event, mousePos);

    if (khachHangSelector.getIsOpen()) return;

    // --- Column 2 ---
    sanPhamTabs.handleEvent(event, mousePos);
    
    if (event.getIf<sf::Event::TextEntered>()) {
        searchSanPham.handleEvent(event);
        loadSanPhamList();
        
        // ✅ THÊM: Xử lý input số lượng trong giỏ hàng
        for (size_t i = 0; i < itemsInCart.size(); ++i) {
            if (itemsInCart[i].qtyInput.getFocus()) {
                itemsInCart[i].qtyInput.handleEvent(event);
            }
        }
    } else if (auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        searchSanPham.handleEvent(event);
        
        // ✅ THÊM: Xử lý phím trong InputBox số lượng
        for (size_t i = 0; i < itemsInCart.size(); ++i) {
            if (itemsInCart[i].qtyInput.getFocus()) {
                itemsInCart[i].qtyInput.handleEvent(event);
                
                // ✅ Enter để cập nhật số lượng
                if (keyEvent->code == sf::Keyboard::Key::Enter) {
                    try {
                        int newQty = std::stoi(itemsInCart[i].qtyInput.getString());
                        if (newQty >= 1) {
                            handleQtyChange(i, newQty);
                        }
                    } catch (...) {
                        errorMessage.setString("So luong khong hop le!");
                    }
                }
            }
        }
    }
    
    if (auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            // ✅ Click vào InputBox số lượng
            for (size_t i = 0; i < itemsInCart.size(); ++i) {
                if (itemsInCart[i].qtyInput.isMouseOver(mousePos)) {
                    itemsInCart[i].qtyInput.setFocus(true);
                } else {
                    itemsInCart[i].qtyInput.setFocus(false);
                }
            }
            
            // Click thêm sản phẩm
            if (sanPhamListBg.getGlobalBounds().contains(sf::Vector2f(mousePos))) {
                if (hoveredSanPhamIndex != -1) {
                    if (sanPhamTabs.getActiveTab() == 0 && hoveredSanPhamIndex < dsHangHoa.size()) {
                        handleAddHangHoa(dsHangHoa[hoveredSanPhamIndex]);
                    } else if (hoveredSanPhamIndex < dsGoiTap.size()) {
                        handleAddGoiTap(dsGoiTap[hoveredSanPhamIndex]);
                    }
                }
            }
        }
    }
    
    // ✅ Xử lý nút +/- trong giỏ hàng
    for (size_t i = 0; i < itemsInCart.size(); ++i) {
        if (itemsInCart[i].uiInitialized) {
            itemsInCart[i].btnMinus.handleEvent(event, mousePos);
            itemsInCart[i].btnPlus.handleEvent(event, mousePos);
        }
    }
    
    confirmPayButton.handleEvent(event, mousePos);
    cancelButton.handleEvent(event, mousePos);
}

// ... (Cac ham update() va drawContent() ban can tu hoan thien
//      de ve 3 cot va cap nhat hover cho 2 danh sach) ...
// GUI/Popups/CartPopup.cpp
// ... (them vao file da co)

// ========================================================================
// HAM UPDATE VA DRAW (HOAN CHINH)
// ========================================================================

void CartPopup::update(sf::Vector2i mousePos) {
    if (!isVisible) return;
    BasePopup::update(mousePos);

    khachHangSelector.update(mousePos);
    phuongThucSelector.update(mousePos);

    if (khachHangSelector.getIsOpen()) {
        hoveredSanPhamIndex = -1;
        hoveredCartItemIndex = -1;
        return;
    }

    searchSanPham.update(sf::Time::Zero);
    
    // ✅ Update tất cả CartItem UI
    for (size_t i = 0; i < itemsInCart.size(); ++i) {
        if (!itemsInCart[i].uiInitialized) {
            setupCartItemUI(itemsInCart[i], i);
        }
        
        itemsInCart[i].btnMinus.update(mousePos);
        itemsInCart[i].btnPlus.update(mousePos);
        itemsInCart[i].qtyInput.update(sf::Time::Zero);
    }
    
    // Update hover index
    hoveredSanPhamIndex = -1;
    sf::FloatRect sanPhamBounds = sanPhamListBg.getGlobalBounds();
    if (sanPhamBounds.contains(sf::Vector2f(mousePos))) {
        float listY = sanPhamBounds.position.y + 10;
        float rowHeight = 35;
        int index = static_cast<int>((mousePos.y - listY) / rowHeight);

        if (sanPhamTabs.getActiveTab() == 0) {
            if (index >= 0 && index < dsHangHoa.size()) hoveredSanPhamIndex = index;
        } else {
            if (index >= 0 && index < dsGoiTap.size()) hoveredSanPhamIndex = index;
        }
    }

    confirmPayButton.update(mousePos);
    cancelButton.update(mousePos);
}

void CartPopup::drawContent(sf::RenderTarget& target) {
    // Ve 3 cot
    drawCol1(target);
    drawCol2(target);
    drawCol3(target);

    // Ve cac nut dieu khien
    target.draw(errorMessage);
    confirmPayButton.draw(target);
    cancelButton.draw(target);
}

// --- HAM VE 3 COT (HELPER) ---

void CartPopup::drawCol1(sf::RenderTarget& target) {
    khachHangSelector.draw(target);
    target.draw(nhanVienText);
    phuongThucSelector.draw(target);
}

void CartPopup::drawCol2(sf::RenderTarget& target) {
    sanPhamTabs.draw(target);
    searchSanPham.draw(target);
    target.draw(sanPhamListBg);

    // Ve danh sach san pham
    sf::Text itemText(font, "", 16);
    itemText.setFillColor(Config::TextNormal);
    
    sf::FloatRect bounds = sanPhamListBg.getGlobalBounds();
    float pX = bounds.position.x + 10;
    float pY = bounds.position.y + 10;
    float rowHeight = 35;

    if (sanPhamTabs.getActiveTab() == 0) { // Hang Hoa
        for (size_t i = 0; i < dsHangHoa.size(); ++i) {
            HangHoa* hh = dsHangHoa[i];
            float currentY = pY + i * rowHeight;
            if (currentY > bounds.position.y + bounds.size.y - rowHeight) break; // Khong ve tran

            if (i == hoveredSanPhamIndex) {
                sf::RectangleShape hoverBg(sf::Vector2f(bounds.size.x, rowHeight));
                hoverBg.setPosition(sf::Vector2f(bounds.position.x, currentY - 5));
                hoverBg.setFillColor(Config::CardDark);
                target.draw(hoverBg);
            }
            
            std::string s = hh->getTenHH() + " (" + std::to_string(hh->getSoLuongCon()) + " con lai)";
            itemText.setString(sf::String::fromUtf8(s.begin(), s.end()));
            itemText.setPosition(sf::Vector2f(pX, currentY));
            target.draw(itemText);
            
            s = std::to_string((int)hh->getGia()) + " VND";
            itemText.setString(sf::String::fromUtf8(s.begin(), s.end()));
            itemText.setPosition(sf::Vector2f(pX + bounds.size.x - 120, currentY));
            target.draw(itemText);
        }
    } else { // Goi Tap
        for (size_t i = 0; i < dsGoiTap.size(); ++i) {
            GoiTap* gt = dsGoiTap[i];
            float currentY = pY + i * rowHeight;
            if (currentY > bounds.position.y + bounds.size.y - rowHeight) break;

            if (i == hoveredSanPhamIndex) {
                sf::RectangleShape hoverBg(sf::Vector2f(bounds.size.x, rowHeight));
                hoverBg.setPosition(sf::Vector2f(bounds.position.x, currentY - 5));
                hoverBg.setFillColor(Config::CardDark);
                target.draw(hoverBg);
            }
            
            itemText.setString(sf::String::fromUtf8(gt->getTenGoi().begin(), gt->getTenGoi().end()));
            itemText.setPosition(sf::Vector2f(pX, currentY));
            target.draw(itemText);
            
            std::string s = std::to_string((int)gt->getGia()) + " VND";
            itemText.setString(sf::String::fromUtf8(s.begin(), s.end()));
            itemText.setPosition(sf::Vector2f(pX + bounds.size.x - 120, currentY));
            target.draw(itemText);
        }
    }
}

void CartPopup::drawCol3(sf::RenderTarget& target) {
    target.draw(cartListBg);
    
    sf::Text itemText(font, "", 14);
    itemText.setFont(font);
    itemText.setCharacterSize(14);
    
    sf::Text priceText(font, "", 12);
    priceText.setFont(font);
    priceText.setCharacterSize(12);
    priceText.setFillColor(Config::TextMuted);
    
    sf::Text deleteText(font, "X", 14);
    deleteText.setFont(font);
    deleteText.setCharacterSize(14);
    deleteText.setString("X");
    deleteText.setFillColor(Config::Danger);

    sf::FloatRect bounds = cartListBg.getGlobalBounds();
    float pX = bounds.position.x + 10;
    float pY = bounds.position.y + 10;
    float rowHeight = 60; // ✅ Tăng height vì có nút

    // Ve cac item trong gio hang
    for (size_t i = 0; i < itemsInCart.size(); ++i) {
        CartItem& item = itemsInCart[i];
        float currentY = pY + i * rowHeight;
        
        if (currentY > bounds.position.y + bounds.size.y - rowHeight) break;

        // ✅ Vẽ background hover (nếu cần)
        
        // ✅ Dòng 1: STT. Tên sản phẩm
        std::string s = std::to_string(i + 1) + ". " + item.ten;
        itemText.setString(sf::String::fromUtf8(s.begin(), s.end()));
        itemText.setPosition(sf::Vector2f(pX, currentY));
        itemText.setFillColor(Config::TextNormal);
        target.draw(itemText);
        
        // ✅ Dòng 2: Đơn giá
        s = "Đơn giá: " + std::to_string((int)item.donGia) + " VND";
        priceText.setString(sf::String::fromUtf8(s.begin(), s.end()));
        priceText.setPosition(sf::Vector2f(pX + 15, currentY + 18));
        target.draw(priceText);
        
        // ✅ Dòng 3: Nút [-] [SL] [+]
        if (item.uiInitialized) {
            item.btnMinus.draw(target);
            item.qtyInput.draw(target);
            item.btnPlus.draw(target);
        }
        
        // ✅ Nút Xóa (góc phải trên)
        deleteText.setPosition(sf::Vector2f(pX + bounds.size.x - 30, currentY + 5));
        target.draw(deleteText);
    }
    
    // Ve tong tien
    target.draw(tongText);
    target.draw(giamGiaText);
    target.draw(finalTotalText);
}

// THÊM 3 HÀM NÀY VÀO TỆP CartPopup.cpp

void CartPopup::handleAddHangHoa(HangHoa* hh) {
    if (hh == nullptr || hh->getSoLuongCon() <= 0) return;

    // Kiem tra xem hang hoa da co trong gio chua
    for (auto& item : itemsInCart) {
        if (!item.isGoiTap && item.id == hh->getID()) {
            if (hh->getSoLuongCon() > 0) {
                item.soLuong++;
                hh->setSoLuongCon(hh->getSoLuongCon() - 1); // Tru ton kho
                tinhTong();
            }
            return; // Da tang so luong
        }
    }

    // Neu chua co, them moi vao gio
    CartItem newItem;
    newItem.id = hh->getID();
    newItem.ten = hh->getTenHH();
    newItem.donGia = hh->getGia();
    newItem.soLuong = 1;
    newItem.isGoiTap = false;
    newItem.hhData = hh; // Luu con tro HangHoa de tra hang
    
    itemsInCart.push_back(newItem);
    hh->setSoLuongCon(hh->getSoLuongCon() - 1); // Tru ton kho
    tinhTong();
}

void CartPopup::handleAddGoiTap(GoiTap* gt) {
    if (gt == nullptr) return;

    // ✅ Kiểm tra xem gói tập đã có trong giỏ chưa
    for (auto& item : itemsInCart) {
        if (item.isGoiTap && item.id == gt->getID()) {
            errorMessage.setString("Goi tap da co trong gio hang.");
            return; 
        }
    }

    // ✅ Thêm mới vào giỏ
    CartItem newItem;
    newItem.id = gt->getID();
    newItem.ten = gt->getTenGoi();
    newItem.donGia = gt->getGia();
    newItem.soLuong = 1; // ✅ Luôn là 1
    newItem.isGoiTap = true;
    newItem.hhData = nullptr;
    newItem.uiInitialized = false; // Sẽ init trong update()
    
    itemsInCart.push_back(newItem);
    tinhTong();
    errorMessage.setString(""); // Clear error
}

void CartPopup::handleRemoveCartItem(int index) {
    if (index < 0 || index >= itemsInCart.size()) return;

    // Lay item chuan bi xoa
    CartItem& item = itemsInCart[index];

    // QUAN TRONG: Tra lai so luong ton kho neu la HangHoa
    if (!item.isGoiTap && item.hhData != nullptr) {
        item.hhData->setSoLuongCon(item.hhData->getSoLuongCon() + item.soLuong);
    }

    // Xoa khoi gio hang
    itemsInCart.erase(itemsInCart.begin() + index);
    
    // Tinh lai tong
    tinhTong();
}

void CartPopup::setupCartItemUI(CartItem& item, size_t index) {
    if (item.uiInitialized) return; // Tránh init lại
    
    sf::FloatRect bounds = cartListBg.getGlobalBounds();
    float pX = bounds.position.x + 10;
    float pY = bounds.position.y + 10;
    float rowHeight = 60; // Tăng height vì có nút
    float currentY = pY + index * rowHeight;
    
    // ✅ Nút Minus (-)
    item.btnMinus.setup("-", font);
    item.btnMinus.setSize(30, 30);
    item.btnMinus.setPosition(pX + bounds.size.x - 150, currentY + 25);
    item.btnMinus.setOnClick([this, index]() {
        if (index < itemsInCart.size()) {
            int newQty = itemsInCart[index].soLuong - 1;
            if (newQty >= 1) {
                handleQtyChange(index, newQty);
            }
        }
    });
    
    // ✅ InputBox số lượng
    item.qtyInput.setup("", font, false);
    item.qtyInput.setSize(50, 30);
    item.qtyInput.setPosition(pX + bounds.size.x - 115, currentY + 25);
    item.qtyInput.setString(std::to_string(item.soLuong));
    
    // ✅ Nút Plus (+)
    item.btnPlus.setup("+", font);
    item.btnPlus.setSize(30, 30);
    item.btnPlus.setPosition(pX + bounds.size.x - 60, currentY + 25);
    item.btnPlus.setOnClick([this, index]() {
        if (index < itemsInCart.size()) {
            int newQty = itemsInCart[index].soLuong + 1;
            
            // ✅ Kiểm tra tồn kho nếu là Hàng Hóa
            if (!itemsInCart[index].isGoiTap && itemsInCart[index].hhData != nullptr) {
                if (itemsInCart[index].hhData->getSoLuongCon() < 1) {
                    errorMessage.setString("Khong du hang trong kho!");
                    return;
                }
            }
            
            // ✅ Gói tập chỉ được mua 1
            if (itemsInCart[index].isGoiTap && newQty > 1) {
                errorMessage.setString("GGoi tap chi duoc mua 1!");
                return;
            }
            
            handleQtyChange(index, newQty);
        }
    });
    
    item.uiInitialized = true;
}

void CartPopup::handleQtyChange(size_t index, int newQty) {
    if (index >= itemsInCart.size() || newQty < 1) return;
    
    CartItem& item = itemsInCart[index];
    int oldQty = item.soLuong;
    int delta = newQty - oldQty;
    
    // ✅ Nếu là Hàng Hóa, cập nhật tồn kho
    if (!item.isGoiTap && item.hhData != nullptr) {
        int available = item.hhData->getSoLuongCon();
        
        if (delta > 0) { // Tăng số lượng
            if (available < delta) {
                errorMessage.setString("Khong du hang! Con " + std::to_string(available));
                return;
            }
            item.hhData->setSoLuongCon(available - delta);
        } else { // Giảm số lượng
            item.hhData->setSoLuongCon(available - delta); // delta âm → cộng lại
        }
    }
    
    // ✅ Cập nhật số lượng
    item.soLuong = newQty;
    item.qtyInput.setString(std::to_string(newQty));
    
    // ✅ Tính lại tổng
    tinhTong();
    
    errorMessage.setString(""); // Clear error
}