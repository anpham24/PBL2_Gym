#include "../../Include/UI/DropdownMenu.h"

DropdownMenu::DropdownMenu(Font& f, const string& labelStr, const vector<string>& opts,
                           float x, float y, float width, float height)
    : font(f), options(opts), selectedIndex(0), isExpanded(false),
      currentText(f, opts.empty() ? "" : opts[0], 18), label(f, labelStr, 16),
      boxX(x), boxY(y), boxWidth(width), boxHeight(height)
{
    box.setSize({width, height});
    box.setPosition({x, y});
    box.setFillColor(Color(240, 240, 240));
    box.setOutlineColor(Color(200, 200, 200));
    box.setOutlineThickness(2);

    label.setFillColor(Color(100, 100, 100));
    label.setPosition({x, y - 25});

    currentText.setFillColor(Color(50, 50, 50));
    currentText.setPosition({x + 15, y + 15});

    // Create option boxes
    for (size_t i = 0; i < options.size(); i++) {
        RectangleShape optBox({width, height});
        optBox.setPosition({x, y + height * (i + 1)});
        optBox.setFillColor(Color(250, 250, 250));
        optBox.setOutlineColor(Color(200, 200, 200));
        optBox.setOutlineThickness(1);
        optionBoxes.push_back(optBox);

        Text optText(font, options[i], 18);
        optText.setFillColor(Color(50, 50, 50));
        optText.setPosition({x + 15, y + height * (i + 1) + 15});
        optionTexts.push_back(optText);
    }
}

void DropdownMenu::handleEvent(const Event& event, Vector2f mousePos) {
    if (event.is<Event::MouseButtonPressed>()) {
        auto* mouseEvent = event.getIf<Event::MouseButtonPressed>();
        if (mouseEvent && mouseEvent->button == Mouse::Button::Left) {
            // Check main box click
            if (box.getGlobalBounds().contains(mousePos)) {
                isExpanded = !isExpanded;
                return;
            }

            // Check option clicks when expanded
            if (isExpanded) {
                for (size_t i = 0; i < optionBoxes.size(); i++) {
                    if (optionBoxes[i].getGlobalBounds().contains(mousePos)) {
                        selectedIndex = i;
                        currentText.setString(options[i]);
                        isExpanded = false;
                        return;
                    }
                }
            }

            // Click outside - collapse
            isExpanded = false;
        }
    }

    // Hover effects
    if (isExpanded) {
        for (size_t i = 0; i < optionBoxes.size(); i++) {
            if (optionBoxes[i].getGlobalBounds().contains(mousePos)) {
                optionBoxes[i].setFillColor(Color(230, 240, 250));
            } else {
                optionBoxes[i].setFillColor(Color(250, 250, 250));
            }
        }
    }
}

void DropdownMenu::draw(RenderWindow& window) {
    window.draw(label);
    window.draw(box);
    window.draw(currentText);

    // Draw arrow indicator
    Text arrow(font, isExpanded ? "▲" : "▼", 16);
    arrow.setFillColor(Color(100, 100, 100));
    Vector2f pos = box.getPosition();
    Vector2f size = box.getSize();
    arrow.setPosition({pos.x + size.x - 30, pos.y + 15});
    window.draw(arrow);

    // Draw options if expanded
    if (isExpanded) {
        for (size_t i = 0; i < optionBoxes.size(); i++) {
            window.draw(optionBoxes[i]);
            window.draw(optionTexts[i]);
        }
    }
}

int DropdownMenu::getSelectedIndex() const { 
    return selectedIndex; 
}

string DropdownMenu::getSelectedValue() const { 
    if (selectedIndex >= 0 && selectedIndex < options.size()) {
        return options[selectedIndex];
    }
    return "";
}

void DropdownMenu::setSelectedIndex(int idx) {
    if (idx >= 0 && idx < options.size()) {
        selectedIndex = idx;
        currentText.setString(options[idx]);
    }
}

void DropdownMenu::setSize(Vector2f newSize) {
    boxWidth = newSize.x;
    boxHeight = newSize.y;
    
    box.setSize(newSize);

    float x = box.getPosition().x;
    float y = box.getPosition().y;
    float width = newSize.x;
    float height = newSize.y;

    currentText.setPosition({x + 15, y + 15});

    // Re-calculate options
    optionBoxes.clear();
    optionTexts.clear();
    
    for (size_t i = 0; i < options.size(); i++) {
        RectangleShape optBox({width, height});
        optBox.setPosition({x, y + height * (i + 1)});
        optBox.setFillColor(Color(250, 250, 250));
        optBox.setOutlineColor(Color(200, 200, 200));
        optBox.setOutlineThickness(1);
        optionBoxes.push_back(optBox);

        Text optText(font, options[i], 18);
        optText.setFillColor(Color(50, 50, 50));
        optText.setPosition({x + 15, y + height * (i + 1) + 15});
        optionTexts.push_back(optText);
    }
}

void DropdownMenu::setPosition(float x, float y) {
    boxX = x;
    boxY = y;
    
    box.setPosition({x, y});
    label.setPosition({x, y - 25});
    currentText.setPosition({x + 15, y + 15});

    float width = box.getSize().x;
    float height = box.getSize().y;
    
    for (size_t i = 0; i < optionBoxes.size(); i++) {
        optionBoxes[i].setPosition({x, y + height * (i + 1)});
        optionTexts[i].setPosition({x + 15, y + height * (i + 1) + 15});
    }
}

void DropdownMenu::updateOptions(const vector<string>& newOptions) {
    options = newOptions;
    selectedIndex = 0;
    
    if (!options.empty()) {
        currentText.setString(options[0]);
    } else {
        currentText.setString("");
    }

    // Recreate option boxes and texts
    optionBoxes.clear();
    optionTexts.clear();

    float x = boxX;
    float y = boxY;
    float width = boxWidth;
    float height = boxHeight;

    for (size_t i = 0; i < options.size(); i++) {
        RectangleShape optBox({width, height});
        optBox.setPosition({x, y + height * (i + 1)});
        optBox.setFillColor(Color(250, 250, 250));
        optBox.setOutlineColor(Color(200, 200, 200));
        optBox.setOutlineThickness(1);
        optionBoxes.push_back(optBox);

        Text optText(font, options[i], 18);
        optText.setFillColor(Color(50, 50, 50));
        optText.setPosition({x + 15, y + height * (i + 1) + 15});
        optionTexts.push_back(optText);
    }
}

bool DropdownMenu::isOpen() const {
    return isExpanded;
}