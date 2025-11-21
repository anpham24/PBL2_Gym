// GUI/Components/InputBox.cpp
#include "InputBox.h"

InputBox::InputBox() 
    : text(nullptr), // SFML 3.0: sf::Text co default constructor
      placeholder(nullptr),
      cursorPosition(0), 
      isFocused(false), 
      isPassword(false) 
{
    // Thiet lap 2 diem cua dau nhay
    cursor[0].color = Config::TextNormal;
    cursor[1].color = Config::TextNormal;
}

InputBox::~InputBox() {
    delete text;
    delete placeholder;
}

void InputBox::setup(const std::string& placeholderStr, sf::Font& font, bool isPass) {
    this->isPassword = isPass;
    this->cursorPosition = 0;
    this->stringValue = "";
    
    box.setSize(sf::Vector2f(200, 30));
    box.setFillColor(Config::CardDark);
    box.setOutlineThickness(2);
    box.setOutlineColor(Config::CardLight);

    text = new sf::Text(font, "", 16);
    text->setFillColor(Config::TextNormal);

    text->setFont(font);
    text->setString("");
    text->setCharacterSize(16);
    text->setFillColor(Config::TextNormal);

    placeholder = new sf::Text(font, placeholderStr, 16);
    placeholder->setFillColor(Config::TextMuted);
    placeholder->setString(placeholderStr);
    placeholder->setCharacterSize(16);
    placeholder->setFillColor(Config::TextMuted);

    // Thiet lap vi tri ban dau (se duoc ghi de boi setPosition)
    setPosition(0, 0);
}

void InputBox::setPosition(float x, float y) {
    box.setPosition(sf::Vector2f(x, y));
    
    // Thêm dòng tính toán này vào TRƯỚC khi bạn dùng textY
    float textY = box.getPosition().y + (box.getSize().y - (text ? text->getCharacterSize() : 16)) / 2.0f - 2;

    // Sau đó mới dùng textY:
    if (text) text->setPosition(sf::Vector2f(box.getPosition().x + 10, textY));
    if (placeholder) placeholder->setPosition(sf::Vector2f(box.getPosition().x + 10, textY));
    updateCursorVisual();
}

void InputBox::setSize(float w, float h) {
    box.setSize(sf::Vector2f(w, h));
    // Cap nhat lai vi tri text/placeholder
    setPosition(box.getPosition().x, box.getPosition().y);
}

void InputBox::handleEvent(sf::Event event) {
    if (!isFocused) return; // Chi xu ly neu dang duoc focus

    // --- 1. Xu ly nhap van ban ---
    // *** SFML 3.0 API CHANGE ***
    if (auto* textEvent = event.getIf<sf::Event::TextEntered>()) {
        
        // Xu ly Backspace
        if (textEvent->unicode == 8) { 
            if (cursorPosition > 0 && !stringValue.empty()) {
                stringValue.erase(cursorPosition - 1, 1);
                cursorPosition--;
                resetCursorBlink();
            }
        }
        // Bo qua Tab (9) va Enter (13)
        else if (textEvent->unicode == 9 || textEvent->unicode == 13) {
            // Khong lam gi ca
        }
        // Xu ly cac ky tu co the in (ASCII >= 32)
        else if (textEvent->unicode >= 32) {
            stringValue.insert(cursorPosition, 1, static_cast<char>(textEvent->unicode));
            cursorPosition++;
            resetCursorBlink();
        }
    }
    
    // --- 2. Xu ly cac phim dieu khien ---
    // *** SFML 3.0 API CHANGE ***
    else if (auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        
        // *** SFML 3.0 API CHANGE ***
        // Su dung sf::Keyboard::Key::...
        switch (keyEvent->code) {
        case sf::Keyboard::Key::Left:
            if (cursorPosition > 0) {
                cursorPosition--;
                resetCursorBlink();
            }
            break;
        case sf::Keyboard::Key::Right:
            if (cursorPosition < stringValue.length()) {
                cursorPosition++;
                resetCursorBlink();
            }
            break;
        case sf::Keyboard::Key::Delete:
            if (cursorPosition < stringValue.length()) {
                stringValue.erase(cursorPosition, 1);
                resetCursorBlink();
            }
            break;
        case sf::Keyboard::Key::Home:
            cursorPosition = 0;
            resetCursorBlink();
            break;
        case sf::Keyboard::Key::End:
            cursorPosition = stringValue.length();
            resetCursorBlink();
            break;
        default:
            break;
        }
    }
}

void InputBox::update(sf::Time dt) {
    if (isFocused) {
        box.setOutlineColor(Config::AccentCyan); // Mau khi focus
        
        // Logic nhap nhay dau nhay
        // *** SFML 3.0 API CHANGE ***
        // Su dung sf::Time thay vi float
        sf::Time blinkTime = cursorClock.getElapsedTime();
        if (blinkTime.asSeconds() > 1.0f) {
            cursorClock.restart();
        }
        
        // Hien 0.5s, an 0.5s
        bool showCursor = blinkTime.asSeconds() < 0.5f;
        cursor[0].color.a = showCursor ? 255 : 0;
        cursor[1].color.a = showCursor ? 255 : 0;
    } else {
        box.setOutlineColor(Config::CardLight); // Mau khi khong focus
        cursor[0].color.a = 0; // An dau nhay
        cursor[1].color.a = 0;
    }
    
    updateTextDisplay();
    updateCursorVisual();
}

void InputBox::draw(sf::RenderTarget& target) {
    target.draw(box);

    if (stringValue.empty() && !isFocused) {
        // SỬA Ở ĐÂY: Dùng *placeholder
        if (placeholder) target.draw(*placeholder); 
    }

    // SỬA Ở ĐÂY: Dùng *text
    if (text) target.draw(*text);

    // Dòng này là Lỗi 3 (xem bên dưới)
    target.draw(cursor, 2, sf::PrimitiveType::Lines); 
}

std::string InputBox::getString() const {
    return stringValue;
}

void InputBox::setString(const std::string& str) {
    stringValue = str;
    cursorPosition = stringValue.length();
    updateTextDisplay();
}

void InputBox::setFocus(bool focus) {
    isFocused = focus;
    if (focus) {
        resetCursorBlink();
    }
}

bool InputBox::getFocus() const {
    return isFocused;
}

bool InputBox::isMouseOver(sf::Vector2i mousePos) {
    sf::Vector2f mousePosF(mousePos);
    return box.getGlobalBounds().contains(mousePosF);
}

void InputBox::setPasswordMode(bool isPass) {
    isPassword = isPass;
    updateTextDisplay(); // Cap nhat hien thi ngay lap tuc
}

// --- Ham private ---

void InputBox::updateTextDisplay() {
    if (isPassword) {
        maskedValue = "";
        for (size_t i = 0; i < stringValue.length(); ++i) {
            maskedValue += '*';
        }
        text->setString(maskedValue);
    } else {
        text->setString(stringValue);
    }
}

void InputBox::updateCursorVisual() {
    // Tim vi tri X cua ky tu tai `cursorPosition`
    // *** SFML 3.0 API CHANGE ***
    // findCharacterPos() van hoat dong nhu cu
    sf::Vector2f charPos = text->findCharacterPos(cursorPosition);
    
    float cursorX = charPos.x;
    
    // Canh chinh dau nhay theo chieu doc
    float cursorY = text->getPosition().y;
    float height = text->getCharacterSize() + 4.0f;

    cursor[0].position = sf::Vector2f(cursorX, cursorY - 2);
    cursor[1].position = sf::Vector2f(cursorX, cursorY + height - 2);
}

void InputBox::resetCursorBlink() {
    cursorClock.restart();
}

// --- THÊM VÀO CUỐI TỆP InputBox.cpp ---

// --- DÁN 2 HÀM NÀY VÀO CUỐI TỆP InputBox.cpp ---

InputBox::InputBox(const InputBox& other)
    : box(other.box),
      stringValue(other.stringValue),
      maskedValue(other.maskedValue),
      cursorPosition(other.cursorPosition),
      isFocused(other.isFocused),
      isPassword(other.isPassword),
      cursorClock(other.cursorClock)
{
    // Sao chép mảng 'cursor'
    cursor[0] = other.cursor[0];
    cursor[1] = other.cursor[1];

    // Sao chép sâu (Deep Copy) 2 con trỏ
    if (other.text != nullptr) {
        this->text = new sf::Text(*other.text);
    } else {
        this->text = nullptr;
    }

    if (other.placeholder != nullptr) {
        this->placeholder = new sf::Text(*other.placeholder);
    } else {
        this->placeholder = nullptr;
    }
}

InputBox& InputBox::operator=(const InputBox& other) {
    if (this == &other) {
        return *this;
    }

    box = other.box;
    stringValue = other.stringValue;
    maskedValue = other.maskedValue;
    cursorPosition = other.cursorPosition;
    isFocused = other.isFocused;
    isPassword = other.isPassword;
    cursorClock = other.cursorClock;
    
    cursor[0] = other.cursor[0];
    cursor[1] = other.cursor[1];

    // Xóa con trỏ cũ
    delete text;
    delete placeholder;

    // Sao chép sâu (Deep Copy)
    if (other.text != nullptr) {
        this->text = new sf::Text(*other.text);
    } else {
        this->text = nullptr;
    }

    if (other.placeholder != nullptr) {
        this->placeholder = new sf::Text(*other.placeholder);
    } else {
        this->placeholder = nullptr;
    }

    return *this;
}