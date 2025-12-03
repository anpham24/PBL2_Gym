// GUI/Popups/CartPopup.cpp
#include "CartPopup.h"
#include "HopDongService.h"
#include "HoaDonService.h"
#include "DateTimeUils.h"
#include "QuanLy.h"
#include "HoiVien.h"
#include <iostream>
// #include <algorithm>

// ========================================================================
// PhuongThucSelector
// ========================================================================
PhuongThucSelector::PhuongThucSelector(sf::Font &font)
    : font(font), selected(""), label(font, "P.Thuc TT:", 16), btnTienMat(), btnChuyenKhoan()
{
    label.setFillColor(Config::TextMuted);
    btnTienMat.setup("Tien Mat", font);
    btnChuyenKhoan.setup("Chuyen Khoan", font);
    btnTienMat.setSize(120, 35);
    btnChuyenKhoan.setSize(120, 35);
    btnTienMat.setOnClick([this]()
                          { setSelected("Tien Mat"); });
    btnChuyenKhoan.setOnClick([this]()
                              { setSelected("Chuyen Khoan"); });
}

void PhuongThucSelector::setup(float x, float y)
{
    label.setPosition(sf::Vector2f(x, y + 5));
    btnTienMat.setPosition(x + 100, y);
    btnChuyenKhoan.setPosition(x + 230, y);
}

void PhuongThucSelector::draw(sf::RenderTarget &target)
{
    target.draw(label);
    btnTienMat.draw(target);
    btnChuyenKhoan.draw(target);
}

void PhuongThucSelector::handleEvent(sf::Event event, sf::Vector2i mousePos)
{
    btnTienMat.handleEvent(event, mousePos);
    btnChuyenKhoan.handleEvent(event, mousePos);
}

void PhuongThucSelector::update(sf::Vector2i mousePos)
{
    btnTienMat.update(mousePos);
    btnChuyenKhoan.update(mousePos);
}

void PhuongThucSelector::setSelected(const std::string &pt)
{
    selected = pt;
    if (pt == "Tien Mat")
    {
        btnTienMat.setColors(Config::AccentCyan, Config::AccentCyan);
        btnChuyenKhoan.setColors(Config::CardLight, Config::CardDark);
    }
    else
    {
        btnTienMat.setColors(Config::CardLight, Config::CardDark);
        btnChuyenKhoan.setColors(Config::AccentCyan, Config::AccentCyan);
    }
}

// ========================================================================
// CartPopup - GIỮ NGUYÊN CONSTRUCTOR CŨ
// ========================================================================
CartPopup::CartPopup(App &app)
    : BasePopup(app, "Tao Hoa Don Moi"),
      onSuccessCallback(nullptr),
      tongHoaDon(0), giamGia(0), tongThanhToan(0),
      khachHangSelector(app.getGlobalFont(), "Tim khach hang (ID/Ten)..."),
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
        (app.getWindow().getSize().y - popupH) / 2.0f));

    title.setPosition(sf::Vector2f(popupPanel.getPosition().x + 20, popupPanel.getPosition().y + 15));
    closeButton.setPosition(popupPanel.getPosition().x + popupW - 40, popupPanel.getPosition().y + 15);

    float pX = popupPanel.getPosition().x;
    float pY = popupPanel.getPosition().y;
    float pTop = pY + 80;
    float pBottom = pY + popupH - 80;

    // ✅ Column 1 (25%) - NHÂN VIÊN Ở TRÊN, KHÁCH HÀNG Ở DƯỚI
    float col1X = pX + 20;
    float col1Width = popupW * 0.25f - 20;

    // Nhân viên ở đầu
    nhanVienText.setFillColor(Config::TextMuted);
    nhanVienText.setPosition(sf::Vector2f(col1X, pTop));

    // Phương thức thanh toán
    phuongThucSelector.setup(col1X, pTop + 40);

    // ✅ Khách hàng ở DƯỚI (không bị che)
    khachHangSelector.setSize(col1Width, 35);
    khachHangSelector.setPosition(col1X, pTop + 90);
    khachHangSelector.setOnSelect([this](HoiVien *hv)
                                  { this->tinhTong(); });

    // ✅ Column 2 (45% - San Pham)
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

    // Column 3 (30% - Gio Hang)
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
    cancelButton.setOnClick([this]()
                            { hide(); });

    confirmPayButton.setup("Thanh Toan", font);
    confirmPayButton.setSize(150, 45);
    confirmPayButton.setPosition(pX + popupW - 170, pBottom + 10);
    confirmPayButton.setOnClick([this]()
                                { handleSubmit(); });
}

void CartPopup::show(std::function<void()> onSuccess)
{
    std::cout << "\n🎬 ===== CART POPUP OPENED =====" << std::endl;

    onSuccessCallback = onSuccess;
    itemsInCart.clear();

    // ✅ Load khách hàng (hiện tên + SDT trong dropdown)
    MyVector<HoiVien *> allHV = app.getQuanLy().getDsHoiVien().getAllValues();
    std::cout << "👥 Số khách hàng: " << allHV.size() << std::endl;

    khachHangSelector.setItems(
        allHV,
        [](HoiVien *hv)
        { return hv->getHoTen() + " (" + hv->getSDT() + ")"; } // ✅ Vẫn hiện SDT
    );
    khachHangSelector.clear();

    searchSanPham.setString("");
    searchSanPham.setFocus(false);

    if (app.getCurrentAccount() != nullptr && app.getCurrentAccount()->getLinkedStaff() != nullptr)
    {
        std::string tenNV = app.getCurrentAccount()->getLinkedStaff()->getHoTen();
        nhanVienText.setString("NV: " + sf::String::fromUtf8(tenNV.begin(), tenNV.end()));
    }
    else
    {
        nhanVienText.setString("NV: ADMIN");
    }

    phuongThucSelector.setSelected("Tien Mat");

    loadSanPhamList();
    tinhTong();

    std::cout << "================================\n"
              << std::endl;
    BasePopup::show();
}

void CartPopup::show(HoiVien *hv, std::function<void()> onSuccess)
{
    std::cout << "\n🛒 ===== CART POPUP OPENED (PRE-FILL) =====" << std::endl;

    if (hv != nullptr)
    {
        std::cout << "   Pre-selecting customer: " << hv->getHoTen() << std::endl;
    }

    onSuccessCallback = onSuccess;
    itemsInCart.clear();

    khachHangSelector.setItems(
        app.getQuanLy().getDsHoiVien().getAllValues(),
        [](HoiVien *hv)
        { return hv->getHoTen() + " (" + hv->getSDT() + ")"; } // ✅ Vẫn hiện SDT
    );

    if (hv != nullptr)
    {
        khachHangSelector.setSelected(hv);
        std::cout << "   ✅ Customer pre-selected: " << hv->getHoTen() << std::endl;
    }
    else
    {
        khachHangSelector.clear();
    }

    searchSanPham.setString("");
    searchSanPham.setFocus(false);

    if (app.getCurrentAccount() != nullptr && app.getCurrentAccount()->getLinkedStaff() != nullptr)
    {
        std::string tenNV = app.getCurrentAccount()->getLinkedStaff()->getHoTen();
        nhanVienText.setString("NV: " + sf::String::fromUtf8(tenNV.begin(), tenNV.end()));
    }
    else
    {
        nhanVienText.setString("NV: ADMIN");
    }

    phuongThucSelector.setSelected("Tien Mat");

    loadSanPhamList();
    tinhTong();

    std::cout << "========================================\n"
              << std::endl;
    BasePopup::show();
}

void CartPopup::show(HoiVien *hv, const std::string &goiID, std::function<void()> onSuccess)
{
    std::cout << "\n🛒 ===== CART POPUP (RENEW) =====" << std::endl;
    std::cout << "   Customer: " << (hv ? hv->getHoTen() : "NULL") << std::endl;
    std::cout << "   Package: " << goiID << std::endl;

    onSuccessCallback = onSuccess;
    itemsInCart.clear();

    khachHangSelector.setItems(
        app.getQuanLy().getDsHoiVien().getAllValues(),
        [](HoiVien *hv)
        { return hv->getHoTen() + " (" + hv->getSDT() + ")"; } // ✅ Vẫn hiện SDT
    );

    if (hv != nullptr)
    {
        khachHangSelector.setSelected(hv);
    }

    searchSanPham.setString("");

    GoiTap *gt = app.getQuanLy().getGoiTap(goiID);
    if (gt != nullptr)
    {
        CartItem item;
        item.id = gt->getID();
        item.ten = gt->getTenGoi();
        item.donGia = gt->getGia();
        item.soLuong = 1;
        item.isGoiTap = true;
        itemsInCart.push_back(item);

        std::cout << "   ✅ Package added to cart: " << item.ten << std::endl;
    }

    loadSanPhamList();
    tinhTong();

    BasePopup::show();
}

void CartPopup::hide()
{
    BasePopup::hide();
    itemsInCart.clear();
}

void CartPopup::loadSanPhamList()
{
    dsHangHoa.clear();
    dsGoiTap.clear();
    std::string term = searchSanPham.getString();

    std::cout << "\n🔍 ===== LOAD SAN PHAM =====" << std::endl;
    std::cout << "Tab: " << (sanPhamTabs.getActiveTab() == 0 ? "Hang Hoa" : "Goi Tap") << std::endl;
    std::cout << "Search term: \"" << term << "\"" << std::endl;

    if (sanPhamTabs.getActiveTab() == 0)
    { // Hàng Hóa
        const MyVector<HangHoa *> &ds = app.getQuanLy().getDsHangHoa();
        std::cout << "📦 Tong Hang Hoa: " << ds.size() << std::endl;

        for (size_t i = 0; i < ds.size(); ++i)
        {
            HangHoa *hh = ds[i];

            if (hh->getIsActive() && hh->getSoLuongCon() > 0)
            {
                // ✅ Linear Search (không dùng std::transform + find)
                bool match = false;

                if (term.empty())
                {
                    match = true;
                }
                else
                {
                    match = StringUtils::contains(hh->getTenHH(), term);
                }

                if (match)
                {
                    dsHangHoa.push_back(hh);
                }
            }
        }
        std::cout << "✅ Loaded " << dsHangHoa.size() << " Hang Hoa" << std::endl;
    }
    else
    { // Gói Tập
        const MyVector<GoiTap *> &ds = app.getQuanLy().getDsGoiTap();
        std::cout << "🎫 Tong Goi Tap: " << ds.size() << std::endl;

        for (size_t i = 0; i < ds.size(); ++i)
        {
            GoiTap *gt = ds[i];

            if (gt->getIsActive())
            {
                // ✅ Linear Search
                bool match = false;

                if (term.empty())
                {
                    match = true;
                }
                else
                {
                    match = StringUtils::contains(gt->getTenGoi(), term);
                }

                if (match)
                {
                    dsGoiTap.push_back(gt);
                }
            }
        }
        std::cout << "✅ Loaded " << dsGoiTap.size() << " Goi Tap" << std::endl;
    }
    std::cout << "===========================\n"
              << std::endl;
}

void CartPopup::tinhTong()
{
    // 1. Tinh tong tien hang (Raw Total)
    // (Viec nay van phai lam o UI vi gio hang dang la bien cuc bo itemsInCart)
    tongHoaDon = 0;
    for (const auto &item : itemsInCart)
    {
        tongHoaDon += item.donGia * item.soLuong;
    }

    // 2. Goi Service de tinh Giam Gia va Thanh Toan
    HoiVien *hv = khachHangSelector.getSelected();

    // Ham nay se cap nhat bien this->giamGia va this->tongThanhToan
    HoaDonService::tinhHoaDon(tongHoaDon, hv, this->giamGia, this->tongThanhToan);

    // 3. Cap nhat UI
    tongText.setString("Tong: " + std::to_string((long long)tongHoaDon) + " VND");
    giamGiaText.setString("Giam gia: - " + std::to_string((long long)giamGia) + " VND");
    finalTotalText.setString("Thanh toan: " + std::to_string((long long)tongThanhToan) + " VND");
}

CartItem *CartPopup::getCartItem(const std::string &id, bool isGoiTap)
{
    for (auto &item : itemsInCart)
    {
        if (item.id == id && item.isGoiTap == isGoiTap)
        {
            return &item;
        }
    }
    return nullptr;
}

void CartPopup::modifyQuantity(HangHoa *hh, int delta)
{
    CartItem *item = getCartItem(hh->getID(), false);

    if (item == nullptr)
    {
        if (delta > 0)
        {
            CartItem newItem;
            newItem.id = hh->getID();
            newItem.ten = hh->getTenHH();
            newItem.donGia = hh->getGia();
            newItem.soLuong = 1;
            newItem.isGoiTap = false;
            newItem.hhData = hh;
            itemsInCart.push_back(newItem);
        }
    }
    else
    {
        int newQty = item->soLuong + delta;
        if (newQty > hh->getSoLuongCon())
        {
            errorMessage.setString("Khong du hang!");
        }
        else if (newQty <= 0)
        {
            for (size_t i = 0; i < itemsInCart.size(); ++i)
            {
                if (itemsInCart[i].id == hh->getID() && !itemsInCart[i].isGoiTap)
                {
                    itemsInCart.erase(itemsInCart.begin() + i);
                    break;
                }
            }
        }
        else
        {
            item->soLuong = newQty;
        }
    }
    tinhTong();
}

void CartPopup::toggleGoiTap(GoiTap *gt)
{
    CartItem *item = getCartItem(gt->getID(), true);
    if (item)
    {
        for (size_t i = 0; i < itemsInCart.size(); ++i)
        {
            if (itemsInCart[i].id == gt->getID() && itemsInCart[i].isGoiTap)
            {
                itemsInCart.erase(itemsInCart.begin() + i);
                break;
            }
        }
    }
    else
    {
        CartItem newItem;
        newItem.id = gt->getID();
        newItem.ten = gt->getTenGoi();
        newItem.donGia = gt->getGia();
        newItem.soLuong = 1;
        newItem.isGoiTap = true;
        itemsInCart.push_back(newItem);
    }
    tinhTong();
}

void CartPopup::handleRemoveCartItem(int index)
{
    if (index >= 0 && index < itemsInCart.size())
    {
        itemsInCart.erase(itemsInCart.begin() + index);
        tinhTong();
    }
}

void CartPopup::handleSubmit()
{
    std::cout << "\n🔔 ===== HANDLE SUBMIT =====" << std::endl;

    HoiVien *hv = khachHangSelector.getSelected();
    NhanVien *nv = app.getCurrentAccount()->getLinkedStaff();

    if (hv == nullptr)
    {
        errorMessage.setString("❌ Chua chon khach hang!");
        return;
    }

    if (itemsInCart.empty())
    {
        errorMessage.setString("❌ Gio hang trong!");
        return;
    }

    std::string now = DateTimeUtils::getCurrentDateTime().substr(0, 10);
    std::string nvID = (nv != nullptr) ? nv->getID() : "ADMIN";

    // ✅ Tạo Hóa Đơn
    HoaDon *hd = HoaDonService::taoHoaDon(nvID, hv->getID(), now, phuongThucSelector.selected);

    if (hd == nullptr)
    {
        errorMessage.setString("❌ Loi tao hoa don!");
        return;
    }

    std::cout << "✅ Invoice created: " << hd->getID() << std::endl;

    // ✅ Thêm items vào hóa đơn + TẠO HỢP ĐỒNG NẾU LÀ GÓI TẬP
    for (size_t i = 0; i < itemsInCart.size(); ++i)
    {
        const auto &item = itemsInCart[i];

        if (item.isGoiTap)
        {
            std::cout << "   [" << i << "] Package: " << item.ten << " x" << item.soLuong << std::endl;

            // Thêm vào hóa đơn
            HoaDonService::themGoiTapVaoHoaDon(hd->getID(), item.id, item.soLuong, item.donGia);

            // ✅ TẠO HỢP ĐỒNG CHO MỖI GÓI TẬP
            GoiTap *gt = app.getQuanLy().getGoiTap(item.id);
            if (gt != nullptr)
            {
                // Tính ngày hết hạn = ngày hiện tại + thời gian gói
                std::string ngayHetHan = DateTimeUtils::addDays(now, gt->getThoiGian());

                std::cout << "      🔨 Creating contract:" << std::endl;
                std::cout << "         Start: " << now << std::endl;
                std::cout << "         End: " << ngayHetHan << std::endl;
                std::cout << "         Package: " << gt->getTenGoi() << std::endl;

                // Tạo hợp đồng
                HopDongService::themHopDong(
                    hv->getID(),      // Hội viên
                    item.id,          // Gói tập
                    nvID,             // Nhân viên
                    now,              // Ngày đăng ký
                    ngayHetHan,       // Ngày hết hạn
                    gt->getSoBuoiPT() // Số buổi PT
                );

                // ✅ Cộng số buổi PT vào Hội viên
                int soBuoiPT = gt->getSoBuoiPT();
                hv->setSoBuoiPT(hv->getSoBuoiPT() + soBuoiPT);
            }
            else
            {
                std::cerr << "      ❌ GoiTap not found: " << item.id << std::endl;
            }
        }
        else
        {
            std::cout << "   [" << i << "] Product: " << item.ten << " x" << item.soLuong << std::endl;
            HoaDonService::themHangHoaVaoHoaDon(hd->getID(), item.id, item.soLuong, item.donGia);
        }
    }

    // Lưu giảm giá
    hd->setGiamGia(giamGia);
    hd->setDaThanhToan(true);

    // Cộng điểm
    int points = static_cast<int>(tongThanhToan / Config::MONEY_TO_POINT_RATIO);
    hv->setPoint(hv->getPoint() + points);

    app.getQuanLy().setDirty(true);

    if (onSuccessCallback)
    {
        onSuccessCallback();
    }

    hide();
}

void CartPopup::drawCol1(sf::RenderTarget &target)
{
    target.draw(nhanVienText);
    phuongThucSelector.draw(target);
    khachHangSelector.draw(target);
}

void CartPopup::drawCol2(sf::RenderTarget &target)
{
    sanPhamTabs.draw(target);
    searchSanPham.draw(target);
    target.draw(sanPhamListBg);

    sf::FloatRect bounds = sanPhamListBg.getGlobalBounds();
    float pX = bounds.position.x + 10;
    float pY = bounds.position.y + 10;
    float rowHeight = 50;
    float width = bounds.size.x - 20;

    sf::Text txt(font, "", 14);
    txt.setFillColor(Config::TextNormal);

    sf::RectangleShape btnRect(sf::Vector2f(30, 30));
    btnRect.setOutlineThickness(1);
    btnRect.setOutlineColor(Config::CardLight);

    if (sanPhamTabs.getActiveTab() == 0)
    { // Hang Hoa
        for (size_t i = 0; i < dsHangHoa.size(); ++i)
        {
            float curY = pY + i * rowHeight;
            if (curY + rowHeight > bounds.position.y + bounds.size.y)
                break;

            HangHoa *hh = dsHangHoa[i];
            CartItem *inCart = getCartItem(hh->getID(), false);
            int qty = inCart ? inCart->soLuong : 0;
            int stock = hh->getSoLuongCon();

            txt.setString(hh->getTenHH());
            txt.setPosition(sf::Vector2f(pX, curY + 5));
            target.draw(txt);

            std::string priceStr = std::to_string((long long)hh->getGia()) + " VND";
            txt.setString(priceStr);
            txt.setPosition(sf::Vector2f(pX, curY + 25));
            txt.setFillColor(Config::TextMuted);
            txt.setCharacterSize(12);
            target.draw(txt);
            txt.setCharacterSize(14);
            txt.setFillColor(Config::TextNormal);

            float ctrlX = pX + width - 110;

            btnRect.setPosition(sf::Vector2f(ctrlX - 10, curY));
            btnRect.setFillColor(Config::CardDark);
            target.draw(btnRect);
            txt.setString("-");
            txt.setPosition(sf::Vector2f(ctrlX, curY + 5));
            target.draw(txt);

            std::string qtyStr = std::to_string(qty) + "/" + std::to_string(stock);
            txt.setString(qtyStr);
            txt.setPosition(sf::Vector2f(ctrlX + 30, curY + 5));
            target.draw(txt);

            btnRect.setPosition(sf::Vector2f(ctrlX + 80, curY));
            target.draw(btnRect);
            txt.setString("+");
            txt.setPosition(sf::Vector2f(ctrlX + 88, curY + 5));
            target.draw(txt);
        }
    }
    else
    { // Goi Tap
        for (size_t i = 0; i < dsGoiTap.size(); ++i)
        {
            float curY = pY + i * rowHeight;
            if (curY + rowHeight > bounds.position.y + bounds.size.y)
                break;

            GoiTap *gt = dsGoiTap[i];
            bool selected = (getCartItem(gt->getID(), true) != nullptr);

            txt.setString(gt->getTenGoi());
            txt.setPosition(sf::Vector2f(pX, curY + 10));
            target.draw(txt);

            float btnW = 80;
            btnRect.setSize(sf::Vector2f(btnW, 30));
            btnRect.setPosition(sf::Vector2f(pX + width - btnW, curY + 5));
            btnRect.setFillColor(selected ? Config::Danger : Config::Success);
            target.draw(btnRect);

            txt.setString(selected ? "Xoa" : "Them");
            txt.setPosition(sf::Vector2f(pX + width - btnW + 20, curY + 10));
            target.draw(txt);
        }
    }
}

void CartPopup::drawCol3(sf::RenderTarget &target)
{
    target.draw(cartListBg);

    sf::Text itemText(font, "", 14);
    sf::Text deleteText(font, "X", 14);
    deleteText.setFillColor(Config::Danger);

    sf::FloatRect bounds = cartListBg.getGlobalBounds();
    float pX = bounds.position.x + 10;
    float pY = bounds.position.y + 10;
    float rowHeight = 50;

    for (size_t i = 0; i < itemsInCart.size(); ++i)
    {
        const auto &item = itemsInCart[i];
        float currentY = pY + i * rowHeight;
        if (currentY > bounds.position.y + bounds.size.y - rowHeight)
            break;

        if (i == hoveredCartItemIndex)
        {
            sf::RectangleShape hoverBg(sf::Vector2f(bounds.size.x - 20, rowHeight - 5));
            hoverBg.setPosition(sf::Vector2f(pX, currentY));
            hoverBg.setFillColor(Config::CardDark);
            target.draw(hoverBg);
        }

        // Dòng 1: Tên
        std::string s = std::to_string(i + 1) + ". " + item.ten;
        itemText.setString(sf::String::fromUtf8(s.begin(), s.end()));
        itemText.setPosition(sf::Vector2f(pX, currentY));
        itemText.setFillColor(Config::TextNormal);
        target.draw(itemText);

        // ✅ Dòng 2: SL x Giá = TỔNG
        long long total = (long long)(item.soLuong * item.donGia);
        s = "   " + std::to_string(item.soLuong) + " x " +
            std::to_string((long long)item.donGia) + " = " +
            std::to_string(total) + " VND";
        itemText.setString(sf::String::fromUtf8(s.begin(), s.end()));
        itemText.setPosition(sf::Vector2f(pX, currentY + 20));
        itemText.setFillColor(Config::TextMuted);
        itemText.setCharacterSize(12);
        target.draw(itemText);
        itemText.setCharacterSize(14);

        deleteText.setPosition(sf::Vector2f(pX + bounds.size.x - 40, currentY + 15));
        target.draw(deleteText);
    }

    target.draw(tongText);
    target.draw(giamGiaText);
    target.draw(finalTotalText);
}

void CartPopup::handleEvent(sf::Event event, sf::Vector2i mousePos)
{
    if (!isVisible)
        return;
    BasePopup::handleEvent(event, mousePos);

    khachHangSelector.handleEvent(event, mousePos);
    phuongThucSelector.handleEvent(event, mousePos);
    if (khachHangSelector.getIsOpen())
        return;

    // ✅ FIX: Lưu tab cũ để detect thay đổi
    static int previousTab = 0;
    int currentTab = sanPhamTabs.getActiveTab();

    sanPhamTabs.handleEvent(event, mousePos);

    // ✅ FIX: Nếu chuyển tab → Load lại sản phẩm
    if (currentTab != sanPhamTabs.getActiveTab())
    {
        std::cout << "🔄 Tab changed: " << currentTab << " → " << sanPhamTabs.getActiveTab() << std::endl;
        loadSanPhamList();
        previousTab = sanPhamTabs.getActiveTab();
    }

    // ✅ Click vào search box
    if (auto *mouseEvent = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mouseEvent->button == sf::Mouse::Button::Left)
        {
            if (searchSanPham.isMouseOver(mousePos))
            {
                searchSanPham.setFocus(true);
            }
            else
            {
                searchSanPham.setFocus(false);
            }
        }
    }

    // ✅ Nhập text vào search
    if (event.getIf<sf::Event::TextEntered>())
    {
        if (searchSanPham.getFocus())
        {
            searchSanPham.handleEvent(event);
            loadSanPhamList(); // ✅ Load lại khi search
        }
    }

    // ✅ Phím trong search
    if (auto *keyEvent = event.getIf<sf::Event::KeyPressed>())
    {
        if (searchSanPham.getFocus())
        {
            searchSanPham.handleEvent(event);
            loadSanPhamList(); // ✅ Load lại khi search
        }
    }

    if (auto *mouseEvent = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mouseEvent->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mPos(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

            if (sanPhamListBg.getGlobalBounds().contains(mPos))
            {
                float listY = sanPhamListBg.getGlobalBounds().position.y + 10;
                float rowHeight = 50;
                int idx = static_cast<int>((mPos.y - listY) / rowHeight);

                float width = sanPhamListBg.getSize().x - 20;
                float pX = sanPhamListBg.getPosition().x + 10;

                if (sanPhamTabs.getActiveTab() == 0)
                { // Hang Hoa
                    if (idx >= 0 && idx < static_cast<int>(dsHangHoa.size()))
                    {
                        float ctrlX = pX + width - 110;
                        float curY = listY + idx * rowHeight;

                        sf::FloatRect minusBtn(
                            sf::Vector2f(ctrlX - 10, curY),
                            sf::Vector2f(30, 30));
                        if (minusBtn.contains(mPos))
                        {
                            modifyQuantity(dsHangHoa[idx], -1);
                        }

                        sf::FloatRect plusBtn(
                            sf::Vector2f(ctrlX + 80, curY),
                            sf::Vector2f(30, 30));
                        if (plusBtn.contains(mPos))
                        {
                            modifyQuantity(dsHangHoa[idx], 1);
                        }
                    }
                }
                else
                { // Goi Tap
                    if (idx >= 0 && idx < static_cast<int>(dsGoiTap.size()))
                    {
                        float btnW = 80;
                        float curY = listY + idx * rowHeight + 5;

                        sf::FloatRect toggleBtn(
                            sf::Vector2f(pX + width - btnW, curY),
                            sf::Vector2f(btnW, 30));
                        if (toggleBtn.contains(mPos))
                        {
                            toggleGoiTap(dsGoiTap[idx]);
                        }
                    }
                }
            }

            if (cartListBg.getGlobalBounds().contains(mPos))
            {
                if (hoveredCartItemIndex != -1)
                {
                    handleRemoveCartItem(hoveredCartItemIndex);
                }
            }
        }
    }

    confirmPayButton.handleEvent(event, mousePos);
    cancelButton.handleEvent(event, mousePos);
}

void CartPopup::update(sf::Vector2i mousePos)
{
    if (!isVisible)
        return;
    BasePopup::update(mousePos);

    khachHangSelector.update(mousePos);
    phuongThucSelector.update(mousePos);
    searchSanPham.update(sf::Time::Zero); // ✅ Update cursor
    confirmPayButton.update(mousePos);
    cancelButton.update(mousePos);

    hoveredCartItemIndex = -1;
    if (cartListBg.getGlobalBounds().contains(sf::Vector2f(mousePos)))
    {
        float listY = cartListBg.getGlobalBounds().position.y + 10;
        float rowHeight = 50;
        int idx = (mousePos.y - listY) / rowHeight;
        if (idx >= 0 && idx < itemsInCart.size())
        {
            hoveredCartItemIndex = idx;
        }
    }
}

void CartPopup::drawContent(sf::RenderTarget &target)
{
    drawCol1(target);
    drawCol2(target);
    drawCol3(target);

    target.draw(errorMessage);
    confirmPayButton.draw(target);
    cancelButton.draw(target);
}