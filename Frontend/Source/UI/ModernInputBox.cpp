#include "../../Include/UI/ModernInputBox.h"

ModernInputBox::ModernInputBox(Font& f, const string& labelStr, const string& placeholderStr, 
                               float x, float y, float width, float height, bool password)
    : font(f), inputText(""), isActive(false), isPassword(password), cursorPosition(0), 
      accumulatedTime(0), cursorVisible(true), showPlaceholder(true), showPassword(false),
      text(f, "", 18), label(f, labelStr, 16), placeholder(f, placeholderStr, 18),
      toggleButton({30, height - 20}), toggleText(f, "O", 14)
{
    box.setSize(Vector2f(width, height));
    box.setPosition({x, y});
    box.setFillColor(Color(240, 240, 240));
    box.setOutlineColor(Color(200, 200, 200));
    box.setOutlineThickness(2);

    label.setFillColor(Color(100, 100, 100));
    label.setPosition({x, y - 25});

    text.setFillColor(Color(50, 50, 50));
    text.setPosition({x + 15, y + 15});

    placeholder.setFillColor(Color(150, 150, 150));
    placeholder.setPosition({x + 15, y + 15});

    // Setup toggle button for password
    toggleButton.setPosition({x + width - 40, y + 10});
    toggleButton.setFillColor(Color(200, 200, 200));
    toggleButton.setOutlineColor(Color(150, 150, 150));
    toggleButton.setOutlineThickness(1);

    toggleText.setFillColor(Color(100, 100, 100));
    FloatRect toggleBounds = toggleText.getLocalBounds();
    toggleText.setOrigin({toggleBounds.size.x / 2.f, toggleBounds.size.y / 2.f});
    toggleText.setPosition({x + width - 25, y + height / 2.f});
}

void ModernInputBox::handleEvent(const Event& event) {
    if (event.is<Event::MouseButtonPressed>()) {
        auto* mouseEvent = event.getIf<Event::MouseButtonPressed>();
        if (mouseEvent && mouseEvent->button == Mouse::Button::Left) {
            Vector2f mousePos(static_cast<float>(mouseEvent->position.x), 
                            static_cast<float>(mouseEvent->position.y));
            bool wasActive = isActive;
            
            // Check if clicking on toggle button (for password fields)
            if (isPassword && toggleButton.getGlobalBounds().contains(mousePos)) {
                showPassword = !showPassword;
                updateDisplayText();
                return;
            }
            
            isActive = box.getGlobalBounds().contains(mousePos);
            
            if (isActive) {
                box.setOutlineColor(Color(70, 130, 180));
                box.setFillColor(Color(250, 250, 250));
                showPlaceholder = false;
                if (!wasActive) cursorPosition = inputText.length();
            } else {
                box.setOutlineColor(Color(200, 200, 200));
                box.setFillColor(Color(240, 240, 240));
                showPlaceholder = inputText.empty();
            }
        }
    }
    else if (event.is<Event::TextEntered>() && isActive) {
        auto* textEvent = event.getIf<Event::TextEntered>();
        if (textEvent->unicode >= 32 && textEvent->unicode < 127) {
            inputText.insert(cursorPosition, 1, static_cast<char>(textEvent->unicode));
            cursorPosition++;
            showPlaceholder = false;
        }
    }
    else if (event.is<Event::KeyPressed>() && isActive) {
        auto* keyEvent = event.getIf<Event::KeyPressed>();
        switch (keyEvent->code) {
            case Keyboard::Key::Backspace:
                if (cursorPosition > 0) {
                    inputText.erase(cursorPosition - 1, 1);
                    cursorPosition--;
                }
                showPlaceholder = inputText.empty();
                break;
            case Keyboard::Key::Delete:
                if (cursorPosition < inputText.length()) {
                    inputText.erase(cursorPosition, 1);
                }
                break;
            case Keyboard::Key::Left:
                if (cursorPosition > 0) cursorPosition--;
                break;
            case Keyboard::Key::Right:
                if (cursorPosition < inputText.length()) cursorPosition++;
                break;
            case Keyboard::Key::Home:
                cursorPosition = 0;
                break;
            case Keyboard::Key::End:
                cursorPosition = inputText.length();
                break;
            default: break;
        }
    }
    updateDisplayText();
}

void ModernInputBox::update(float deltaTime) {
    if (isActive) {
        accumulatedTime += deltaTime;
        if (accumulatedTime >= 0.5f) {
            cursorVisible = !cursorVisible;
            accumulatedTime = 0;
            updateDisplayText();
        }
    }
}

void ModernInputBox::updateDisplayText() {
    string displayText;
    if (isPassword && !showPassword) {
        displayText = string(inputText.length(), '*');
    } else {
        displayText = inputText;
    }
    
    if (isActive && cursorVisible) {
        displayText.insert(cursorPosition, "|");
    }
    text.setString(displayText);
}

void ModernInputBox::draw(RenderWindow& window) {
    window.draw(box);
    window.draw(label);
    
    // Draw toggle button for password fields
    if (isPassword) {
        window.draw(toggleButton);
        toggleText.setString(showPassword ? "-" : "O");
        window.draw(toggleText);
    }
    
    if (showPlaceholder && inputText.empty()) {
        window.draw(placeholder);
    } else {
        window.draw(text);
    }
}

string ModernInputBox::getText() const { 
    return inputText; 
}

void ModernInputBox::clear() { 
    inputText.clear(); 
    cursorPosition = 0;
    showPlaceholder = true;
    updateDisplayText();
}

void ModernInputBox::setText(const string& t) {
    inputText = t;
    cursorPosition = inputText.length();
    showPlaceholder = false;
    updateDisplayText();
}

void ModernInputBox::setActive(bool active) { 
    isActive = active;
    if (active) {
        box.setOutlineColor(Color(70, 130, 180));
        box.setFillColor(Color(250, 250, 250));
        showPlaceholder = false;
        cursorVisible = true;
    } else {
        box.setOutlineColor(Color(200, 200, 200));
        box.setFillColor(Color(240, 240, 240));
        showPlaceholder = inputText.empty();
    }
    updateDisplayText();
}

bool ModernInputBox::getIsActive() const { 
    return isActive; 
}

void ModernInputBox::setSize(Vector2f newSize) {
    box.setSize(newSize);
    
    float x = box.getPosition().x;
    float y = box.getPosition().y;
    float width = newSize.x;
    float height = newSize.y;

    // Recalculate positions based on new size
    text.setPosition({x + 15, y + 15});
    placeholder.setPosition({x + 15, y + 15});

    if (isPassword) {
        toggleButton.setPosition({x + width - 40, y + 10});
        toggleText.setPosition({x + width - 25, y + height / 2.f});
    }
}

void ModernInputBox::setPosition(float x, float y) {
    box.setPosition({x, y});
    label.setPosition({x, y - 25});
    text.setPosition({x + 15, y + 15});
    placeholder.setPosition({x + 15, y + 15});
    
    if (isPassword) {
        float width = box.getSize().x;
        float height = box.getSize().y;
        toggleButton.setPosition({x + width - 40, y + 10});
        toggleText.setPosition({x + width - 25, y + height / 2.f});
    }
}