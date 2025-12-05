// GUI/Components/Checkbox.h
#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <functional>
#include "Config.h" // De lay bang mau

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
    Checkbox();
    ~Checkbox();

    void setup(const std::string& text, sf::Font& font);
    void setPosition(float x, float y);
    void setChecked(bool checked);
    bool getIsChecked() const;
    void setOnToggle(std::function<void(bool)> func);
    void toggle();
    bool isMouseOver(sf::Vector2i mousePos);


    void handleEvent(sf::Event event, sf::Vector2i mousePos);
    void update(sf::Vector2i mousePos);
    void draw(sf::RenderTarget& target);
    void setFocused(bool focused);
    
    Checkbox(const Checkbox& other);

    Checkbox& operator=(const Checkbox& other);
};