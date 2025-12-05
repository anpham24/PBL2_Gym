// GUI/Screens/MainScreen.h
#pragma once
#include "BaseScreen.h"
#include "Sidebar.h"
#include <memory> // De su dung std::unique_ptr

// Forward declarations
class App;
class Sidebar;

// Dinh nghia tat ca cac man hinh con (noi dung) co the hien thi
enum class ContentScreenType {
    HoiVien,
    GoiTap,
    LopHoc,
    HLV,
    MonTap,
    NhanVien,
    HangHoa,
    BanHang,
    BaoCao,
    CaiDat
};

/*
 * Lop MainScreen la man hinh chinh cua ung dung sau khi dang nhap.
 * No chia man hinh thanh 2 phan:
 * 1. Sidebar (ben trai): Thanh dieu huong co dinh.
 * 2. ContentScreen (ben phai): Noi dung thay doi (vi du: bang Hoi Vien).
 */
class MainScreen : public BaseScreen {
private:
    Sidebar sidebar;
    
    std::unique_ptr<BaseScreen> contentScreen;
    
    ContentScreenType currentContentScreen;

public:
    MainScreen(App& app);
    
    void handleEvent(sf::Event event) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderTarget& target) override;

    void changeContentScreen(ContentScreenType type);
};