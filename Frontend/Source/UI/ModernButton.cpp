#include "../../Include/UI/ModernButton.h"

ModernButton::ModernButton(Font& f, const string& buttonText, float x, float y, 
                           float width, float height)
    : font(f), normalColor(Color(70, 130, 180)), hoverColor(Color(100, 160, 210)), 
      clickColor(Color(50, 110, 160)), disabledColor(Color(150, 150, 150)),
      isHovered(false), isClicking(false), isEnabled(true),
      text(f, buttonText, 18)
{
    box.setSize({width, height});
    box.setPosition({x, y});
    box.setFillColor(normalColor);
    
    text.setFillColor(Color::White);
    FloatRect textBounds = text.getLocalBounds();
    text.setOrigin({textBounds.size.x / 2.f, textBounds.size.y / 2.f});
    text.setPosition({x + width / 2.f, y + height / 2.f - 3});
}

void ModernButton::handleEvent(const Event& event, Vector2f mousePos) {
    if (!isEnabled) {
        isHovered = false;
        return;
    }

    bool wasHovered = isHovered;
    isHovered = box.getGlobalBounds().contains(mousePos);
    
    if (isHovered != wasHovered) {
        box.setFillColor(isHovered ? hoverColor : normalColor);
    }

    if (event.is<Event::MouseButtonPressed>() && isHovered) {
        auto* mouseEvent = event.getIf<Event::MouseButtonPressed>();
        if (mouseEvent && mouseEvent->button == Mouse::Button::Left) {
            box.setFillColor(clickColor);
            isClicking = true;
        }
    }
    
    if (event.is<Event::MouseButtonReleased>()) {
        isClicking = false;
        box.setFillColor(isHovered ? hoverColor : normalColor);
    }
}

bool ModernButton::isClicked(const Event& event, Vector2f mousePos) {
    if (!isEnabled) return false;

    if (event.is<Event::MouseButtonReleased>()) {
        auto* mouseEvent = event.getIf<Event::MouseButtonReleased>();
        if (mouseEvent && mouseEvent->button == Mouse::Button::Left) {
            return box.getGlobalBounds().contains(mousePos);
        }
    }
    return false;
}

void ModernButton::draw(RenderWindow& window) {
    window.draw(box);
    window.draw(text);
}

void ModernButton::setPosition(float x, float y) {
    box.setPosition({x, y});
    Vector2f size = box.getSize();
    FloatRect textBounds = text.getLocalBounds();
    text.setOrigin({textBounds.size.x / 2.f, textBounds.size.y / 2.f});
    text.setPosition({x + size.x / 2.f, y + size.y / 2.f - 3});
}

void ModernButton::setText(const string& newText) {
    text.setString(newText);
    Vector2f pos = box.getPosition();
    Vector2f size = box.getSize();
    FloatRect textBounds = text.getLocalBounds();
    text.setOrigin({textBounds.size.x / 2.f, textBounds.size.y / 2.f});
    text.setPosition({pos.x + size.x / 2.f, pos.y + size.y / 2.f - 3});
}

void ModernButton::setEnabled(bool enabled) {
    isEnabled = enabled;
    if (!enabled) {
        box.setFillColor(disabledColor);
        isHovered = false;
        isClicking = false;
    } else {
        box.setFillColor(normalColor);
    }
}

void ModernButton::setColors(Color normal, Color hover, Color click) {
    normalColor = normal;
    hoverColor = hover;
    clickColor = click;
    
    if (!isEnabled) {
        box.setFillColor(disabledColor);
    } else if (isClicking) {
        box.setFillColor(clickColor);
    } else if (isHovered) {
        box.setFillColor(hoverColor);
    } else {
        box.setFillColor(normalColor);
    }
}

bool ModernButton::getIsHovered() const {
    return isHovered;
}