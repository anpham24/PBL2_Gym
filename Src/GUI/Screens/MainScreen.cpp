// GUI/Screens/MainScreen.cpp
#include "MainScreen.h"
#include "../App.h"
#include <iostream>

// --- (MOI) INCLUDE TAT CA CAC MAN HINH CON ---
#include "HoiVienScreen.h"
#include "MonTapScreen.h"
#include "GoiTapScreen.h"
#include "HLVScreen.h"
#include "LopHocScreen.h"
#include "NhanVienScreen.h"
#include "HangHoaScreen.h"
#include "BanHangScreen.h"
#include "BaoCaoScreen.h"
#include "CaiDatScreen.h"

MainScreen::MainScreen(App& app) 
    : BaseScreen(app), 
      sidebar(*this, app), // Khoi tao Sidebar
      currentContentScreen(ContentScreenType::HoiVien) // Mac dinh
{
    // Thiet lap Sidebar dua tren tai khoan hien tai
    sidebar.setup(app.getCurrentAccount());
    
    // Tai man hinh noi dung mac dinh (Hoi Vien)
    changeContentScreen(ContentScreenType::HoiVien);
}

void MainScreen::handleEvent(sf::Event event) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(app.getWindow());

    // 1. Gui su kien cho Sidebar truoc
    sidebar.handleEvent(event, mousePos);
    
    // 2. Gui su kien cho man hinh noi dung hien tai
    if (contentScreen) {
        contentScreen->handleEvent(event);
    }
}

void MainScreen::update(sf::Time dt) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(app.getWindow());

    // 1. Cap nhat Sidebar
    sidebar.update(mousePos);
    
    // 2. Cap nhat man hinh noi dung hien tai
    if (contentScreen) {
        contentScreen->update(dt);
    }
}

void MainScreen::draw(sf::RenderTarget& target) {
    // 1. Ve Sidebar (luon luon o ben trai)
    sidebar.draw(target);
    
    // 2. Ve man hinh noi dung hien tai (ben phai sidebar)
    if (contentScreen) {
        contentScreen->draw(target);
    }
}

/**
 * @brief (DA CAP NHAT) Ham nay tao va chuyen doi cac man hinh noi dung
 */
void MainScreen::changeContentScreen(ContentScreenType type) {
    // Giai phong man hinh noi dung cu
    contentScreen.reset();
    currentContentScreen = type;

    // Tao man hinh noi dung moi dua tren `type`
    switch (type) {
    case ContentScreenType::HoiVien:
        contentScreen = std::make_unique<HoiVienScreen>(app);
        break;
    case ContentScreenType::GoiTap:
        contentScreen = std::make_unique<GoiTapScreen>(app);
        break;
    case ContentScreenType::LopHoc:
        contentScreen = std::make_unique<LopHocScreen>(app);
        break;
    case ContentScreenType::HLV:
        contentScreen = std::make_unique<HLVScreen>(app);
        break;
    case ContentScreenType::NhanVien:
        contentScreen = std::make_unique<NhanVienScreen>(app);
        break;
    case ContentScreenType::HangHoa:
        contentScreen = std::make_unique<HangHoaScreen>(app);
        break;
    case ContentScreenType::BanHang:
        contentScreen = std::make_unique<BanHangScreen>(app);
        break;
    case ContentScreenType::BaoCao:
        // (Bao ve them: Neu la Staff, khong tao man hinh nay)
        if (app.getCurrentAccount()->getAccountType() != AccountType::ADMIN) {
            std::cerr << "Loi: Staff co gang truy cap Bao Cao!" << std::endl;
        } else {
            contentScreen = std::make_unique<BaoCaoScreen>(app);
        }
        break;
    case ContentScreenType::CaiDat:
        contentScreen = std::make_unique<CaiDatScreen>(app);
        break;
    case ContentScreenType::MonTap:
        contentScreen = std::make_unique<MonTapScreen>(app);
        break;
    default:
        std::cout << "Man hinh chua duoc thuc hien!" << std::endl;
        // (Ban co the tao mot DefaultScreen de hien thi)
        break;
    }
}