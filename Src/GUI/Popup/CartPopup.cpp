// GUI/Popups/CartPopup.cpp
#include "CartPopup.h"
#include "HoaDonService.h"
#include "DateTimeUils.h"
#include "QuanLy.h"
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
      khachHangSelector(app.getGlobalFont(), "Tim khach hang (SDT/Ten)..."),
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
    std::string term = searchSanPham.getString();//.toAnsiString();
    // (nen convert term sang chu thuong)
    
    if (sanPhamTabs.getActiveTab() == 0) { // Tab Hang Hoa
        const MyVector<HangHoa*>& ds = app.getQuanLy().getDsHangHoa();
        for (size_t i = 0; i < ds.size(); ++i) {
            HangHoa* hh = ds[i];
            if (hh->getIsActive() && hh->getSoLuongCon() > 0) {
                std::string tenHH = hh->getTenHH();
                // (convert tenHH sang chu thuong)
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
                // (convert tenGT sang chu thuong)
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
    BasePopup::handleEvent(event, mousePos); // Nut [X]

    // --- Column 1 ---
    khachHangSelector.handleEvent(event, mousePos);
    phuongThucSelector.handleEvent(event, mousePos);

    // Neu dropdown mo, khong xu ly gi them
    if (khachHangSelector.getIsOpen()) return;

    // --- Column 2 ---
    sanPhamTabs.handleEvent(event, mousePos);
    
    if (event.getIf<sf::Event::TextEntered>()) {
        searchSanPham.handleEvent(event);
        loadSanPhamList();
    } else if (event.getIf<sf::Event::KeyPressed>()) {
         searchSanPham.handleEvent(event);
    }
    
    if (auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            // Click them san pham
            if (sanPhamListBg.getGlobalBounds().contains(sf::Vector2f(mousePos))) {
                if (hoveredSanPhamIndex != -1) {
                    if (sanPhamTabs.getActiveTab() == 0 && hoveredSanPhamIndex < dsHangHoa.size()) {
                        handleAddHangHoa(dsHangHoa[hoveredSanPhamIndex]);
                    } else if (hoveredSanPhamIndex < dsGoiTap.size()) {
                        handleAddGoiTap(dsGoiTap[hoveredSanPhamIndex]);
                    }
                }
            }
            // Click xoa item khoi gio
            if (cartListBg.getGlobalBounds().contains(sf::Vector2f(mousePos))) {
                if (hoveredCartItemIndex != -1 && hoveredCartItemIndex < itemsInCart.size()) {
                    handleRemoveCartItem(hoveredCartItemIndex);
                }
            }
        }
    }
    
    // --- Buttons ---
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
    BasePopup::update(mousePos); // Cap nhat nut [X]

    // --- Column 1 ---
    khachHangSelector.update(mousePos);
    phuongThucSelector.update(mousePos);

    // Neu dropdown mo, khong update gi them
    if (khachHangSelector.getIsOpen()) {
        hoveredSanPhamIndex = -1;
        hoveredCartItemIndex = -1;
        return;
    }

    // --- Column 2 ---
    searchSanPham.update(sf::Time::Zero); // Update dau nhay
    
    hoveredSanPhamIndex = -1;
    sf::FloatRect sanPhamBounds = sanPhamListBg.getGlobalBounds();
    if (sanPhamBounds.contains(sf::Vector2f(mousePos))) {
        float listY = sanPhamBounds.position.y + 10;
        float rowHeight = 35;
        int index = static_cast<int>((mousePos.y - listY) / rowHeight);

        // Kiem tra index hop le
        if (sanPhamTabs.getActiveTab() == 0) {
            if (index >= 0 && index < dsHangHoa.size()) hoveredSanPhamIndex = index;
        } else {
            if (index >= 0 && index < dsGoiTap.size()) hoveredSanPhamIndex = index;
        }
    }

    // --- Column 3 ---
    hoveredCartItemIndex = -1;
    sf::FloatRect cartBounds = cartListBg.getGlobalBounds();
    if (cartBounds.contains(sf::Vector2f(mousePos))) {
        float listY = cartBounds.position.y + 10;
        float rowHeight = 40; // Hang trong gio hang cao hon
        int index = static_cast<int>((mousePos.y - listY) / rowHeight);
        
        if (index >= 0 && index < itemsInCart.size()) {
            hoveredCartItemIndex = index;
        }
    }

    // --- Buttons ---
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
    sf::Text priceText(font, "", 14);
    sf::Text deleteText(font, "Xoa", 14);
    deleteText.setFillColor(Config::Danger);

    sf::FloatRect bounds = cartListBg.getGlobalBounds();
    float pX = bounds.position.x + 10;
    float pY = bounds.position.y + 10;
    float rowHeight = 40; // Hang trong gio hang cao hon

    // Ve cac item trong gio hang
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
        
        // Dong 1: STT. Ten
        std::string s = std::to_string(i + 1) + ". " + item.ten;
        itemText.setString(sf::String::fromUtf8(s.begin(), s.end()));
        itemText.setPosition(sf::Vector2f(pX, currentY));
        itemText.setFillColor(Config::TextNormal);
        target.draw(itemText);
        
        // Dong 2: SL x Gia = Tong
        s = std::to_string(item.soLuong) + " x " + std::to_string((int)item.donGia) 
            + " = " + std::to_string((int)(item.soLuong * item.donGia));
        priceText.setString(sf::String::fromUtf8(s.begin(), s.end()));
        priceText.setPosition(sf::Vector2f(pX + 15, currentY + 18));
        priceText.setFillColor(Config::TextMuted);
        target.draw(priceText);
        
        // Nut Xoa
        deleteText.setPosition(sf::Vector2f(pX + bounds.size.x - 40, currentY + 5));
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

    // Kiem tra xem goi tap da co trong gio chua
    for (auto& item : itemsInCart) {
        if (item.isGoiTap && item.id == gt->getID()) {
            // (Thong thuong khong cho them 2 goi tap giong nhau)
            errorMessage.setString("Goi tap nay da co trong gio hang.");
            return; 
        }
    }

    // Them moi vao gio
    CartItem newItem;
    newItem.id = gt->getID();
    newItem.ten = gt->getTenGoi();
    newItem.donGia = gt->getGia();
    newItem.soLuong = 1;
    newItem.isGoiTap = true;
    newItem.hhData = nullptr; // Khong phai HangHoa
    
    itemsInCart.push_back(newItem);
    tinhTong();
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