// GUI/Components/Checkbox.h
#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <functional>
#include "Config.h" // De lay bang mau

/*
 * Lop Checkbox dinh nghia mot o vuong co the duoc tich chon.
 * No xu ly trang thai checked/unchecked va focused.
 */
class Checkbox {
private:
    sf::RectangleShape box;       // O vuong ben ngoai
    sf::RectangleShape checkMark; // Dau tich 'V' (hoac hinh vuong ben trong)
    sf::Text* label;           // Nhan ben canh
    sf::Font* pFont; // Con tro den font toan cuc

    bool isChecked;
    bool isHovered;
    bool isFocused; // De dieu huong bang phim

    // Callback se duoc goi khi trang thai thay doi (true hoac false)
    std::function<void(bool)> onToggle;

public:
    /**
     * @brief Ham dung mac dinh.
     */
    Checkbox();
    ~Checkbox();

    /**
     * @brief Thiet lap cac gia tri cho checkbox.
     * @param text Nhan hien thi ben canh o vuong.
     * @param font Font chu toan cuc tu App.
     */
    void setup(const std::string& text, sf::Font& font);

    /**
     * @brief Dat vi tri cho checkbox.
     * @param x Vi tri X.
     * @param y Vi tri Y.
     */
    void setPosition(float x, float y);

    /**
     * @brief Dat trang thai (da tich hay chua).
     * @param checked true de tich vao o, false de bo tich.
     */
    void setChecked(bool checked);
    
    /**
     * @brief Lay trang thai hien tai.
     */
    bool getIsChecked() const;

    /**
     * @brief Dang ky mot ham de goi khi trang thai thay doi.
     * @param func Ham callback (vi du: [this](bool newState){ ... }).
     */
    void setOnToggle(std::function<void(bool)> func);

    /**
     * @brief Dao nguoc trang thai hien tai (tu tich sang khong hoac nguoc lai).
     */
    void toggle();

    /**
     * @brief Kiem tra xem chuot co dang o tren checkbox hoac nhan khong.
     * @param mousePos Vi tri hien tai cua chuot.
     */
    bool isMouseOver(sf::Vector2i mousePos);

    /**
     * @brief Xu ly su kien (chu yeu la click chuot).
     * @param event Su kien tu vong lap chinh.
     * @param mousePos Vi tri hien tai cua chuot.
     */
    void handleEvent(sf::Event event, sf::Vector2i mousePos);

    /**
     * @brief Cap nhat trang thai (hover).
     * @param mousePos Vi tri hien tai cua chuot.
     */
    void update(sf::Vector2i mousePos);

    /**
     * @brief Ve checkbox len cua so.
     * @param target Cua so (window) de ve.
     */
    void draw(sf::RenderTarget& target);
    
    /**
     * @brief Dat trang thai focus (khi dieu huong bang phim).
     * @param focused true neu duoc chon.
     */
    void setFocused(bool focused);
    
    // 1. Hàm Khởi Tạo Sao Chép
    Checkbox(const Checkbox& other);

    // 2. Hàm Gán Sao Chép
    Checkbox& operator=(const Checkbox& other);
};