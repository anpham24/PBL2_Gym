// GUI/Components/Button.cpp
#include "Button.h"

// SFML 3.0: sf::Text co mot default constructor
Button::Button() :text(nullptr), isHovered(false), isFocused(false) {
    // Ham dung de trong, thiet lap trong setup()
}

Button::~Button() {
    delete text; // "delete" an toàn ngay cả khi text là nullptr
    text = nullptr;
}

Button::Button(const Button& other) 
    // Sao chép các thành viên giá trị
    : shape(other.shape), 
      idleColor(other.idleColor),
      hoverColor(other.hoverColor),
      focusedOutlineColor(other.focusedOutlineColor),
      onClick(other.onClick),
      isHovered(other.isHovered),
      isFocused(other.isFocused)
{
    // Sao chép sâu (Deep Copy) cho con trỏ
    if (other.text != nullptr) {
        // Tạo một đối tượng sf::Text MỚI
        this->text = new sf::Text(*other.text); 
    } else {
        this->text = nullptr;
    }
}

// 2. Hàm Gán Sao Chép (Copy Assignment Operator)
// (Được gọi khi gán Button B cho Button A (A = B), nếu A đã tồn tại)
Button& Button::operator=(const Button& other) {
    if (this == &other) {
        return *this; // Tự gán (a = a), không làm gì cả
    }

    // Sao chép các thành viên giá trị
    shape = other.shape;
    idleColor = other.idleColor;
    hoverColor = other.hoverColor;
    focusedOutlineColor = other.focusedOutlineColor;
    onClick = other.onClick;
    isHovered = other.isHovered;
    isFocused = other.isFocused;

    // Sao chép sâu (Deep Copy) cho con trỏ
    // (Phải xóa 'text' cũ trước khi tạo 'text' mới)
    delete text; 
    
    if (other.text != nullptr) {
        this->text = new sf::Text(*other.text);
    } else {
        this->text = nullptr;
    }

    return *this; // Trả về chính đối tượng này
}

void Button::setup(const std::string& label, sf::Font& font) {
    // Thiet lap mau mac dinh
    idleColor = Config::AccentCyan;
    hoverColor = sf::Color(
        std::min(255, idleColor.r + 30),
        std::min(255, idleColor.g + 30),
        std::min(255, idleColor.b + 30)
    );
    focusedOutlineColor = Config::Warning;

    shape.setSize(sf::Vector2f(150, 40));
    shape.setFillColor(idleColor);

    text = new sf::Text(font, label, 16);

    // text->setFont(font); // Font phai duoc set truoc khi goi getLocalBounds
    // text->setString(label);
    // text->setCharacterSize(16);
    text->setFillColor(Config::NavyBackground);
}

void Button::setPosition(float x, float y) {
shape.setPosition(sf::Vector2f(x, y)); 

    // Thêm kiểm tra "if (text)" để đảm bảo an toàn
    if (text) { 
        // SỬA Ở ĐÂY: Dùng '->' thay vì '.'
        sf::FloatRect textRect = text->getLocalBounds();
        text->setOrigin(
            sf::Vector2f(
                textRect.position.x + textRect.size.x / 2.0f,
                textRect.position.y + textRect.size.y / 2.0f
            )
        );
        text->setPosition(
            sf::Vector2f(
                x + shape.getSize().x / 2.0f,
                y + shape.getSize().y / 2.0f - 2
            )
        );
    }
}

void Button::setSize(float w, float h) {
    shape.setSize(sf::Vector2f(w, h));
    setPosition(shape.getPosition().x, shape.getPosition().y);
}

void Button::setOnClick(std::function<void()> func) {
    this->onClick = func;
}

std::function<void()> Button::getOnClick() const {
    return onClick;
}

bool Button::isMouseOver(sf::Vector2i mousePos) {
    // SFML 3.0: sf::Vector2f co constructor tu sf::Vector2i
    sf::Vector2f mousePosF(mousePos); 
    return shape.getGlobalBounds().contains(mousePosF);
}

void Button::handleEvent(sf::Event event, sf::Vector2i mousePos) {
    // *** SFML 3.0 API CHANGE ***
    // Su dung event.getIf<>() de kiem tra loai su kien
    if (auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
        
        // *** SFML 3.0 API CHANGE ***
        // Su dung sf::Mouse::Button::Left thay vi sf::Mouse::Left
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            if (isMouseOver(mousePos)) {
                click();
            }
        }
    }
}

void Button::update(sf::Vector2i mousePos) {
    isHovered = isMouseOver(mousePos);
    
    if (isHovered) {
        shape.setFillColor(hoverColor);
    } else {
        shape.setFillColor(idleColor);
    }

    if (isFocused) {
        shape.setOutlineThickness(3);
        shape.setOutlineColor(focusedOutlineColor);
    } else {
        shape.setOutlineThickness(0);
    }
}

void Button::draw(sf::RenderTarget& target) {
    target.draw(shape);
    
    // Thêm kiểm tra "if (text)" và SỬA: Dùng '*text'
    if (text) {
        target.draw(*text); // Dùng dấu * để VẼ đối tượng mà con trỏ đang trỏ tới
    }
}

void Button::setFocused(bool focused) {
    this->isFocused = focused;
}

void Button::click() {
    if (onClick) {
        onClick();
    }
}

void Button::setColors(sf::Color idle, sf::Color hover) {
    idleColor = idle;
    hoverColor = hover;
}