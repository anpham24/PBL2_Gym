// GUI/Components/Button.h
#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <functional> // De su dung std::function
#include "Config.h" // De lay bang mau

/*
 * Lop Button dinh nghia mot nut bam co the tuong tac.
 * No xu ly cac trang thai: idle (binh thuong), hover (di chuot qua),
 * va focused (duoc chon bang ban phim).
 */
class Button {
private:
    sf::RectangleShape shape; // Hinh dang cua nut
    sf::Text* text;            // Chu ben trong nut

    // Cac mau sac cho tung trang thai
    sf::Color idleColor;
    sf::Color hoverColor;
    sf::Color focusedOutlineColor; // Mau vien khi duoc chon

    // Ham callback (con tro ham) se duoc goi khi nut duoc click
    std::function<void()> onClick;

    bool isHovered;
    bool isFocused;

public:
    /**
     * @brief Ham dung mac dinh.
     */
    Button();
    ~Button();

    Button(const Button& other); // 1. Hàm Khởi Tạo Sao Chép
    Button& operator=(const Button& other); // 2. Hàm Gán Sao Chép

    /**
     * @brief Thiet lap cac gia tri cho nut.
     * @param label Chu hien thi tren nut.
     * @param font Font chu toan cuc tu App.
     */
    void setup(const std::string& label, sf::Font& font);

    /**
     * @brief Dat vi tri cho nut.
     * @param x Vi tri X.
     * @param y Vi tri Y.
     */
    void setPosition(float x, float y);

    /**
     * @brief Dat kich thuoc cho nut.
     * @param w Chieu rong.
     * @param h Chieu cao.
     */
    void setSize(float w, float h);

    /**
     * @brief Dang ky mot ham de goi khi nut duoc click.
     * @param func Ham callback (vi du: mot lambda [this](){ ... }).
     */
    void setOnClick(std::function<void()> func);

    /**
     * @brief Tra ve ham callback (de kiem tra).
     */
    std::function<void()> getOnClick() const;

    /**
     * @brief Kiem tra xem chuot co dang de len nut hay khong.
     * @param mousePos Vi tri hien tai cua chuot.
     * @return true neu chuot dang o tren nut.
     */
    bool isMouseOver(sf::Vector2i mousePos);

    /**
     * @brief Xu ly su kien cho nut (chu yeu la MouseClick).
     * @param event Su kien tu vong lap chinh.
     * @param mousePos Vi tri hien tai cua chuot.
     */
    void handleEvent(sf::Event event, sf::Vector2i mousePos);

    /**
     * @brief Cap nhat trang thai cua nut (chu yeu la de check hover).
     * @param mousePos Vi tri hien tai cua chuot.
     */
    void update(sf::Vector2i mousePos);

    /**
     * @brief Ve nut len cua so.
     * @param target Cua so (window) de ve.
     */
    void draw(sf::RenderTarget& target);

    /**
     * @brief Dat trang thai focus (khi dieu huong bang phim).
     * @param focused true neu nut duoc chon.
     */
    void setFocused(bool focused);

    /**
     * @brief Gia lap mot cu click (thuong duoc goi bang phim Enter).
     */
    void click();

    /**
     * @brief Thiet lap mau sac tuy chinh cho nut.
     */
    void setColors(sf::Color idle, sf::Color hover);
};