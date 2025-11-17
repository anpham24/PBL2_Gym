// GUI/Components/InputBox.h
#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "Config.h" // De lay bang mau

/*
 * Lop InputBox dinh nghia mot o nhap lieu van ban.
 * No xu ly logic nhap lieu, dau nhay (cursor), 
 * va dieu huong bang phim mui ten.
 */
class InputBox {
private:
    sf::RectangleShape box;     // Hop ben ngoai
    sf::Text* text;              // Van ban hien thi
    sf::Text* placeholder;       // Chu mo khi o trong
    sf::Vertex cursor[2];       // Dau nhay '|' (ve bang 2 diem)
    sf::Clock cursorClock;      // Dong ho de dieu khien nhap nhay

    std::string stringValue;    // Chuoi du lieu goc
    std::string maskedValue;    // Chuoi bi che (VD: "******")
    int cursorPosition;         // Vi tri (index) cua dau nhay trong chuoi
    
    bool isFocused;
    bool isPassword;

    /**
     * @brief Cap nhat sf::Text de hien thi stringValue (hoac maskedValue).
     */
    void updateTextDisplay();

    /**
     * @brief Cap nhat vi tri cua dau nhay '|' tren man hinh.
     */
    void updateCursorVisual();

    /**
     * @brief Reset dong ho nhap nhay (khi go phim hoac click).
     */
    void resetCursorBlink();

public:
    /**
     * @brief Ham dung mac dinh.
     */
    InputBox();
    ~InputBox();

    /**
     * @brief Thiet lap cac gia tri cho o nhap lieu.
     * @param placeholder Chu mo hien thi khi o trong.
     * @param font Font chu toan cuc tu App.
     * @param isPass true neu day la o mat khau.
     */
    void setup(const std::string& placeholder, sf::Font& font, bool isPass = false);
    
    void setPosition(float x, float y);
    void setSize(float w, float h);

    /**
     * @brief Xu ly su kien cho o nhap lieu (nhap phim, dieu huong).
     * @param event Su kien tu vong lap chinh.
     */
    void handleEvent(sf::Event event);

    /**
     * @brief Cap nhat trang thai (chu yeu la dau nhay nhap nhay).
     * @param dt Delta time.
     */
    void update(sf::Time dt);

    /**
     * @brief Ve o nhap lieu len cua so.
     * @param target Cua so (window) de ve.
     */
    void draw(sf::RenderTarget& target);

    std::string getString() const;
    void setString(const std::string& str);

    void setFocus(bool focus);
    bool getFocus() const;

    /**
     * @brief Kiem tra xem chuot co dang o tren o nhap lieu khong.
     * @param mousePos Vi tri hien tai cua chuot.
     * @return true neu chuot dang o tren.
     */
    bool isMouseOver(sf::Vector2i mousePos);

    /**
     * @brief Bat/Tat che do mat khau.
     * @param isPass true de bat che do mat khau (hien thi '*').
     */
    void setPasswordMode(bool isPass);
    
    // 1. Hàm Khởi Tạo Sao Chép
    InputBox(const InputBox& other);

    // 2. Hàm Gán Sao Chép
    InputBox& operator=(const InputBox& other);
};