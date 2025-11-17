// GUI/Components/Pagination.h
#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>
#include <string>
#include "Config.h" // De lay bang mau

/*
 * Lop Pagination quan ly viec dieu huong trang.
 * No hien thi cac nut dieu khien (Dau, Truoc, Sau, Cuoi) va cac so trang.
 * Khi nguoi dung chuyen trang, no se goi mot ham callback.
 */
class Pagination {
private:
    sf::Font& font;
    sf::Vector2f position;
    
    int currentPage;
    int totalPages;
    int itemsPerPage;

    // Callback se duoc goi khi trang thay doi
    std::function<void(int)> onPageChange;

    // Cac thanh phan van ban co the nhap
    sf::Text firstPageText; // "<<"
    sf::Text prevPageText;  // "<"
    sf::Text nextPageText;  // ">"
    sf::Text lastPageText;  // ">>"
    sf::Text pageStatusText; // "Trang 1 / 100"
    
    // Day la danh sach cac so trang dong (VD: "1", "...", "5", "6", "7")
    std::vector<sf::Text> pageNumberTexts;

    // De xu ly hover
    int hoveredPageIndex; // -1 = none, 0 = "<<", 1 = "<", 2..N = so trang, ...

    /**
     * @brief Tao mot doi tuong sf::Text cho viec dieu huong.
     */
    static sf::Text createNavText(sf::Font& font, const std::string& str, bool isInteractive);
    
    /**
     * @brief Tinh toan va cap nhat lai cac so trang can hien thi.
     */
    void updateVisuals();
    
    /**
     * @brief Kiem tra xem chuot dang hover tren muc nao.
     */
    int getHoveredIndex(sf::Vector2i mousePos);

public:
    /**
     * @brief Ham dung.
     * @param font Font toan cuc tu App.
     */
    Pagination(sf::Font& font);
    ~Pagination();

    /**
     * @brief Thiet lap tong so muc va so muc moi trang.
     * Ham nay se tu dong tinh toan tong so trang.
     * @param totalItems Tong so muc (VD: tong so hoi vien).
     * @param itemsPerPage So muc toi da tren 1 trang.
     */
    void setup(int totalItems, int itemsPerPage);

    /**
     * @brief Dang ky ham callback de goi khi chuyen trang.
     * @param func Ham se duoc goi, nhan vao so trang moi (1-based).
     */
    void setOnPageChange(std::function<void(int)> func);

    /**
     * @brief Dat vi tri cho component.
     */
    void setPosition(float x, float y);

    /**
     * @brief Dat trang hien tai mot cach tuong minh.
     * @param page So trang moi (1-based).
     */
    void setCurrentPage(int page);

    int getCurrentPage() const;
    int getTotalPages() const;

    void handleEvent(sf::Event event, sf::Vector2i mousePos);
    void update(sf::Vector2i mousePos);
    void draw(sf::RenderTarget& target);
};