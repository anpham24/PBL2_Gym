// GUI/Components/Checkbox.cpp
#include "Checkbox.h"

// SFML 3.0: sf::Text co default constructor
Checkbox::Checkbox() 
    : label(nullptr), isChecked(false), isHovered(false), isFocused(false), pFont(nullptr) 
{
    // Ham dung de trong
}

Checkbox::~Checkbox() {
    delete label; // "delete" an toàn ngay cả khi label là nullptr
    label = nullptr;
}

void Checkbox::setup(const std::string& text, sf::Font& font) {
    pFont = &font;
    
    box.setSize(sf::Vector2f(20, 20)); // Kich thuoc o vuong
    box.setFillColor(Config::CardDark);
    box.setOutlineThickness(2);
    box.setOutlineColor(Config::CardLight);

    // Dau tich (hinh vuong nho hon ben trong)
    checkMark.setSize(sf::Vector2f(10, 10)); 
    // Mac dinh an di
    checkMark.setFillColor(sf::Color::Transparent); 

    label = new sf::Text(*pFont, text, 16); 

    // SỬA Ở ĐÂY: Dùng '->' (mũi tên)
    label->setFillColor(Config::TextNormal);
    
    setPosition(0, 0); // Dat vi tri mac dinh
}

void Checkbox::setPosition(float x, float y) {
    // Canh chinh o vuong voi text theo chieu doc
    box.setPosition(sf::Vector2f(x, y + 2)); 
    
    // Canh chinh dau tich o giua o vuong
    checkMark.setPosition(sf::Vector2f(
        x + (box.getSize().x - checkMark.getSize().x) / 2.0f,
        y + 2 + (box.getSize().y - checkMark.getSize().y) / 2.0f
    ));
    
    // Dat text ben phai o vuong
    if (label) {
        label->setPosition(sf::Vector2f(x + box.getSize().x + 10, y));
    }
}

void Checkbox::setChecked(bool checked) {
    isChecked = checked;
    // Hien thi hoac an dau tich
    checkMark.setFillColor(isChecked ? Config::AccentCyan : sf::Color::Transparent);
}

bool Checkbox::getIsChecked() const {
    return isChecked;
}

void Checkbox::setOnToggle(std::function<void(bool)> func) {
    onToggle = func;
}

void Checkbox::toggle() {
    setChecked(!isChecked); // Dao nguoc trang thai
    
    // Goi callback neu da dang ky
    if (onToggle) {
        onToggle(isChecked);
    }
}

bool Checkbox::isMouseOver(sf::Vector2i mousePos) {
    sf::Vector2f mousePosF(mousePos);
    
    // Kiem tra ca o vuong va text (nhan chuot vao text cung tinh)
    sf::FloatRect boxBounds = box.getGlobalBounds();
    // SỬA Ở ĐÂY
    sf::FloatRect labelBounds = label ? label->getGlobalBounds() : sf::FloatRect();
    
    return boxBounds.contains(mousePosF) || labelBounds.contains(mousePosF);
}

void Checkbox::handleEvent(sf::Event event, sf::Vector2i mousePos) {
    // *** SFML 3.0 API CHANGE ***
    if (auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            if (isMouseOver(mousePos)) {
                toggle();
            }
        }
    }
}

void Checkbox::update(sf::Vector2i mousePos) {
    isHovered = isMouseOver(mousePos);
    
    if (isFocused) {
        // Neu dang duoc focus (bang phim), luon hien mau
        box.setOutlineColor(Config::AccentCyan);
    } else if (isHovered) {
        // Neu dang di chuot qua
        box.setOutlineColor(Config::Warning); // Mau vang nhat
    } else {
        // Binh thuong
        box.setOutlineColor(Config::CardLight);
    }
}

void Checkbox::draw(sf::RenderTarget& target) {
    target.draw(box);
    target.draw(checkMark);
    // SỬA Ở ĐÂY: Dùng * và kiểm tra
    if (label) {
        target.draw(*label);
    }
}

void Checkbox::setFocused(bool focused) {
    isFocused = focused;
}

// --- THÊM VÀO CUỐI TỆP Checkbox.cpp ---

Checkbox::Checkbox(const Checkbox& other)
    : box(other.box),
      checkMark(other.checkMark),
      pFont(other.pFont),
      onToggle(other.onToggle),
      isChecked(other.isChecked),
      isHovered(other.isHovered),
      isFocused(other.isFocused)
{
    // Sao chép sâu (Deep Copy)
    if (other.label != nullptr) {
        this->label = new sf::Text(*other.label);
    } else {
        this->label = nullptr;
    }
}

Checkbox& Checkbox::operator=(const Checkbox& other) {
    if (this == &other) {
        return *this;
    }

    box = other.box;
    checkMark = other.checkMark;
    pFont = other.pFont;
    onToggle = other.onToggle;
    isChecked = other.isChecked;
    isHovered = other.isHovered;
    isFocused = other.isFocused;

    // Sao chép sâu (Deep Copy)
    delete label;
    if (other.label != nullptr) {
        this->label = new sf::Text(*other.label);
    } else {
        this->label = nullptr;
    }

    return *this;
}