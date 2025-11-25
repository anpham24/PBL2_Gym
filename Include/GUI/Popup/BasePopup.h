// GUI/Popups/BasePopup.h
#pragma once

#include <SFML/Graphics.hpp>
#include "App.h"
#include "Button.h" // De co nut [X]

/*
 * Lop BasePopup la lop co so (Abstract Base Class) cho tat ca cac popup.
 * No quan ly viec hien thi (show/hide), mot lop phu mo (overlay)
 * phia sau, va mot panel chinh o giua.
 *
 * Cac lop con se ke thua va ve noi dung cua minh len panel nay.
 */
class BasePopup {
protected:
    App& app;

    sf::Font& font; // Tham chieu den font toan cuc
    sf::RectangleShape backgroundOverlay; // Lop phu mo
    sf::RectangleShape popupPanel;        // Khung cua so popup
    sf::Text title;
    Button closeButton; // Nut [X] de dong

    bool isVisible;


    /**
     * @brief Ham ao (virtual) ma cac lop con se ve noi dung cua chung.
     * @param target Cua so (window) de ve.
     */
    virtual void drawContent(sf::RenderTarget& target) = 0;

public:
    /**
     * @brief Ham dung.
     * @param app Tham chieu den App chinh.
     * @param titleStr Tieu de cua popup.
     */
    BasePopup(App& app, const std::string& titleStr);
    
    // Ham huy ao
    virtual ~BasePopup();

    /**
     * @brief Hien thi popup.
     */
    virtual void show();

    /**
     * @brief An popup di.
     */
    virtual void hide();

    /**
     * @brief Kiem tra xem popup co dang hien thi hay khong.
     * (Quan trong: De chan click xuyen qua man hinh ben duoi)
     */
    bool getIsVisible() const;

    // Cac ham nay chi duoc goi boi man hinh cha KHI `isVisible` la true
    
    /**
     * @brief Xu ly su kien (cho nut [X] va cac nut cua con).
     */
    virtual void handleEvent(sf::Event event, sf::Vector2i mousePos);
    
    /**
     * @brief Cap nhat (cho nut [X] va cac nut cua con).
     */
    virtual void update(sf::Vector2i mousePos);
    
    /**
     * @brief Ve popup (overlay, panel, va noi dung cua con).
     */
    virtual void draw(sf::RenderTarget& target);
};