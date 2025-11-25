// GUI/Screens/MainScreen.cpp
#include "MainScreen.h"
#include "../App.h"
#include <iostream>

// Include all screens
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
      sidebar(*this, app),
      currentContentScreen(ContentScreenType::HoiVien)
{
    std::cout << "\n🎬 ===== MAIN SCREEN INIT =====" << std::endl;
    
    sidebar.setup(app.getCurrentAccount());
    
    // ✅ FIX: Try-catch để bắt lỗi
    try {
        changeContentScreen(ContentScreenType::HoiVien);
        
        if (contentScreen) {
            std::cout << "✅ Default screen loaded: HoiVien" << std::endl;
        } else {
            std::cerr << "❌ CRITICAL: contentScreen is nullptr!" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "❌ EXCEPTION in changeContentScreen: " << e.what() << std::endl;
    }
    
    std::cout << "================================\n" << std::endl;
}

void MainScreen::handleEvent(sf::Event event) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(app.getWindow());

    sidebar.handleEvent(event, mousePos);
    
    // ✅ FIX: Kiểm tra nullptr TRƯỚC KHI gọi
    if (contentScreen) {
        try {
            contentScreen->handleEvent(event);
        } catch (const std::exception& e) {
            std::cerr << "❌ EXCEPTION in handleEvent: " << e.what() << std::endl;
        }
    } else {
        std::cerr << "⚠️ WARNING: contentScreen is nullptr in handleEvent!" << std::endl;
    }
}

void MainScreen::update(sf::Time dt) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(app.getWindow());

    sidebar.update(mousePos);
    
    // ✅ FIX: Kiểm tra nullptr
    if (contentScreen) {
        try {
            contentScreen->update(dt);
        } catch (const std::exception& e) {
            std::cerr << "❌ EXCEPTION in update: " << e.what() << std::endl;
        }
    }
}

void MainScreen::draw(sf::RenderTarget& target) {
    sidebar.draw(target);
    
    // ✅ FIX: Kiểm tra nullptr
    if (contentScreen) {
        try {
            contentScreen->draw(target);
        } catch (const std::exception& e) {
            std::cerr << "❌ EXCEPTION in draw: " << e.what() << std::endl;
        }
    }
}

void MainScreen::changeContentScreen(ContentScreenType type) {
    std::cout << "\n🔄 Changing screen to: " << static_cast<int>(type) << std::endl;
    
    // Giải phóng màn hình cũ
    contentScreen.reset();
    currentContentScreen = type;

    // ✅ FIX: Try-catch cho TỪNG case
    try {
        switch (type) {
        case ContentScreenType::HoiVien:
            std::cout << "   Loading HoiVienScreen..." << std::endl;
            contentScreen = std::make_unique<HoiVienScreen>(app);
            std::cout << "   ✅ HoiVienScreen loaded" << std::endl;
            break;
            
        case ContentScreenType::GoiTap:
            std::cout << "   Loading GoiTapScreen..." << std::endl;
            contentScreen = std::make_unique<GoiTapScreen>(app);
            std::cout << "   ✅ GoiTapScreen loaded" << std::endl;
            break;
            
        case ContentScreenType::LopHoc:
            std::cout << "   Loading LopHocScreen..." << std::endl;
            contentScreen = std::make_unique<LopHocScreen>(app);
            std::cout << "   ✅ LopHocScreen loaded" << std::endl;
            break;
            
        case ContentScreenType::HLV:
            std::cout << "   Loading HLVScreen..." << std::endl;
            contentScreen = std::make_unique<HLVScreen>(app);
            std::cout << "   ✅ HLVScreen loaded" << std::endl;
            break;
            
        case ContentScreenType::NhanVien:
            std::cout << "   Loading NhanVienScreen..." << std::endl;
            contentScreen = std::make_unique<NhanVienScreen>(app);
            std::cout << "   ✅ NhanVienScreen loaded" << std::endl;
            break;
            
        case ContentScreenType::HangHoa:
            std::cout << "   Loading HangHoaScreen..." << std::endl;
            contentScreen = std::make_unique<HangHoaScreen>(app);
            std::cout << "   ✅ HangHoaScreen loaded" << std::endl;
            break;
            
        case ContentScreenType::BanHang:
            std::cout << "   Loading BanHangScreen..." << std::endl;
            contentScreen = std::make_unique<BanHangScreen>(app);
            std::cout << "   ✅ BanHangScreen loaded" << std::endl;
            break;
            
        case ContentScreenType::BaoCao:
            if (app.getCurrentAccount()->getAccountType() != AccountType::ADMIN) {
                std::cerr << "   ❌ Access denied: Staff cannot access BaoCao" << std::endl;
            } else {
                std::cout << "   Loading BaoCaoScreen..." << std::endl;
                contentScreen = std::make_unique<BaoCaoScreen>(app);
                std::cout << "   ✅ BaoCaoScreen loaded" << std::endl;
            }
            break;
            
        case ContentScreenType::CaiDat:
            std::cout << "   Loading CaiDatScreen..." << std::endl;
            contentScreen = std::make_unique<CaiDatScreen>(app);
            std::cout << "   ✅ CaiDatScreen loaded" << std::endl;
            break;
            
        case ContentScreenType::MonTap:
            std::cout << "   Loading MonTapScreen..." << std::endl;
            contentScreen = std::make_unique<MonTapScreen>(app);
            std::cout << "   ✅ MonTapScreen loaded" << std::endl;
            break;
            
        default:
            std::cerr << "   ❌ Unknown screen type: " << static_cast<int>(type) << std::endl;
            break;
        }
        
    } catch (const std::bad_alloc& e) {
        std::cerr << "❌ MEMORY ERROR in changeContentScreen: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "❌ EXCEPTION in changeContentScreen: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "❌ UNKNOWN EXCEPTION in changeContentScreen!" << std::endl;
    }
    
    // ✅ Verify
    if (contentScreen) {
        std::cout << "✅ Screen change successful" << std::endl;
    } else {
        std::cerr << "❌ CRITICAL: Screen change failed - contentScreen is nullptr!" << std::endl;
    }
}