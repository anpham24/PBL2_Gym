#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <functional>
#include "Config.h"
#include "Utils/MyVector.h"
#include "InputBox.h" // Cần để dùng InputBox

// ========================================================================
// DINH NGHIA ITEM (STRUCT)
// ========================================================================
template <typename T>
struct SelectorItem {
    std::string displayText; 
    T* data; 
};

// ========================================================================
// LOP SELECTOR (HEADER)
// ========================================================================
template <typename T>
class Selector {
private:
    sf::Font& font;
    sf::RectangleShape mainBox; 
    sf::Text selectedText;
    sf::Text placeholderText;

    // --- Phan Dropdown (tha xuong) ---
    sf::RectangleShape dropdownBackground;
    std::vector<sf::Text> dropdownTexts; 
    int hoveredDropdownIndex; 

    // --- Data ---
    std::vector<SelectorItem<T>> allItems; 
    std::vector<SelectorItem<T>*> filteredItems; 
    SelectorItem<T>* currentSelection; 
    
    // --- Trang thai ---
    bool isOpen; 
    InputBox searchBox; 

    // Thêm biến lưu vị trí/kích thước (vì InputBox không có getter)
    sf::Vector2f searchBoxPos;
    sf::Vector2f searchBoxSize;

    std::function<void(T*)> onSelect;

    void filterList();

public:
    Selector(sf::Font& font, const std::string& placeholder);

    void setPosition(float x, float y);
    void setSize(float w, float h);

    void setItems(const MyVector<T*>& data, std::function<std::string(T*)> nameExtractor);
    
    void filterItems(std::function<bool(T*)> predicate);
    
    void setSelected(T* item);
    T* getSelected() const;
    bool getIsOpen() const; // Hàm này bạn đã thêm ở lượt trước
    
    void setOnSelect(std::function<void(T*)> func);
    void clear(); 

    void handleEvent(sf::Event event, sf::Vector2i mousePos);
    void update(sf::Vector2i mousePos);
    void draw(sf::RenderTarget& target);
};

// ========================================================================
// LOP SELECTOR (IMPLEMENTATION)
// ========================================================================

template <typename T>
Selector<T>::Selector(sf::Font& font, const std::string& placeholder)
    : font(font), 
      currentSelection(nullptr), 
      isOpen(false), 
      hoveredDropdownIndex(-1),
      // Sửa 1: Lỗi Constructor sf::Text
      selectedText(font, "", 16), 
      placeholderText(font, sf::String::fromUtf8(placeholder.begin(), placeholder.end()), 16),
      // Sửa 2: Lỗi Constructor InputBox (phải gọi hàm setup bên dưới)
      searchBox() 
{
    mainBox.setSize(sf::Vector2f(300, 35));
    mainBox.setFillColor(Config::CardDark);
    mainBox.setOutlineThickness(2);
    mainBox.setOutlineColor(Config::CardLight);
    
    selectedText.setFillColor(Config::TextNormal);
    placeholderText.setFillColor(Config::TextMuted);
    
    dropdownBackground.setFillColor(Config::CardDark);
    dropdownBackground.setOutlineColor(Config::AccentCyan);
    dropdownBackground.setOutlineThickness(1);
    
    // Sửa 2 (tiếp): Gọi hàm setup() cho searchBox
    searchBox.setup("Tim...", font, false);
}

template <typename T>
void Selector<T>::setPosition(float x, float y) {
    // Sửa 3: Lỗi API setPosition (SFML 3.0)
    mainBox.setPosition(sf::Vector2f(x, y));
    
    float textY = y + (mainBox.getSize().y - selectedText.getCharacterSize()) / 2.0f - 2;
    selectedText.setPosition(sf::Vector2f(x + 10, textY));
    placeholderText.setPosition(sf::Vector2f(x + 10, textY));
    
    dropdownBackground.setPosition(sf::Vector2f(x, y + mainBox.getSize().y + 2));

    // Sửa 4: Lưu vị trí searchBox (vì InputBox không có getter)
    searchBoxPos = sf::Vector2f(x + 5, y + mainBox.getSize().y + 7);
    searchBox.setPosition(searchBoxPos.x, searchBoxPos.y);
}

template <typename T>
void Selector<T>::setSize(float w, float h) {
    mainBox.setSize(sf::Vector2f(w, h));
    dropdownBackground.setSize(sf::Vector2f(w, 190)); 

    // Sửa 4 (tiếp): Lưu kích thước searchBox
    searchBoxSize = sf::Vector2f(w - 10, 30);
    searchBox.setSize(searchBoxSize.x, searchBoxSize.y);
    
    setPosition(mainBox.getPosition().x, mainBox.getPosition().y);
}

template <typename T>
void Selector<T>::setItems(const MyVector<T*>& data, std::function<std::string(T*)> nameExtractor) {
    allItems.clear();
    for (size_t i = 0; i < data.size(); ++i) {
        T* item = data[i];
        if (item != nullptr) {
            allItems.push_back({nameExtractor(item), item});
        }
    }
    filterList(); 
}

template <typename T>
void Selector<T>::filterItems(std::function<bool(T*)> predicate) {
    filteredItems.clear();
    // Sửa 5: Lỗi std::string (xóa .toAnsiString)
    std::string searchTerm = searchBox.getString();

    for (auto& item : allItems) {
        // 1. Loc theo dieu kien ben ngoai
        if (predicate(item.data)) {
            
            // 2. Loc theo search box noi bo
            std::string itemText = item.displayText;
            
            if (searchTerm.empty() || itemText.find(searchTerm) != std::string::npos) {
                filteredItems.push_back(&item);
            }
        }
    }
    
    bool stillValid = false;
    if (currentSelection != nullptr) {
        for (auto* filtered : filteredItems) {
            if (currentSelection == filtered) {
                stillValid = true;
                break;
            }
        }
    }
    
    if (!stillValid) {
        setSelected(nullptr); 
    }
}

template <typename T>
void Selector<T>::filterList() {
    filterItems([](T*){ return true; }); 
}

template <typename T>
void Selector<T>::setSelected(T* item) {
    if (item == nullptr) {
        currentSelection = nullptr;
        selectedText.setString("");
    } else {
        for (auto& selItem : allItems) {
            if (selItem.data == item) {
                currentSelection = &selItem;
                selectedText.setString(sf::String::fromUtf8(currentSelection->displayText.begin(), currentSelection->displayText.end()));
                break;
            }
        }
    }
}

template <typename T>
T* Selector<T>::getSelected() const {
    return (currentSelection != nullptr) ? currentSelection->data : nullptr;
}

template <typename T>
bool Selector<T>::getIsOpen() const {
    return isOpen;
}

template <typename T>
void Selector<T>::setOnSelect(std::function<void(T*)> func) {
    onSelect = func;
}

template <typename T>
void Selector<T>::clear() {
    setSelected(nullptr);
    filterList();
}

template <typename T>
void Selector<T>::handleEvent(sf::Event event, sf::Vector2i mousePos) {
    if (auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePosF(mousePos);
            
            if (mainBox.getGlobalBounds().contains(mousePosF)) {
                isOpen = !isOpen; 
                searchBox.setFocus(isOpen);
                if (isOpen) filterList(); 
            
            } else if (isOpen && dropdownBackground.getGlobalBounds().contains(mousePosF)) {
                if (searchBox.isMouseOver(mousePos)) {
                    searchBox.setFocus(true);
                } else if (hoveredDropdownIndex != -1 && hoveredDropdownIndex < filteredItems.size()) {
                    currentSelection = filteredItems[hoveredDropdownIndex];
                    selectedText.setString(sf::String::fromUtf8(currentSelection->displayText.begin(), currentSelection->displayText.end()));
                    
                    if (onSelect) {
                        onSelect(currentSelection->data); 
                    }
                    
                    isOpen = false;
                    searchBox.setFocus(false);
                }
            } else {
                isOpen = false;
                searchBox.setFocus(false);
            }
        }
    }

    if (isOpen) {
        if (event.getIf<sf::Event::TextEntered>()) {
            searchBox.handleEvent(event);
            filterList(); 
        } else if (event.getIf<sf::Event::KeyPressed>()) {
            searchBox.handleEvent(event);
        }
    }
}

template <typename T>
void Selector<T>::update(sf::Vector2i mousePos) {
    if (isOpen) {
        searchBox.update(sf::Time::Zero); 
        
        hoveredDropdownIndex = -1;
        float pX = dropdownBackground.getPosition().x;

        // Sửa 4 (tiếp): Dùng biến đã lưu
        float pY = searchBoxPos.y + searchBoxSize.y + 5;
        float rowHeight = 30;
        
        for (size_t i = 0; i < filteredItems.size(); ++i) {
            // Sửa 7: Lỗi Constructor sf::FloatRect
            sf::FloatRect rowBounds(
                {pX, pY + i * rowHeight}, 
                {dropdownBackground.getSize().x, rowHeight}
            );
            if (rowBounds.contains(sf::Vector2f(mousePos))) {
                hoveredDropdownIndex = i;
                break;
            }
        }
    }
}

template <typename T>
void Selector<T>::draw(sf::RenderTarget& target) {
    target.draw(mainBox);
    if (currentSelection == nullptr) {
        target.draw(placeholderText);
    } else {
        target.draw(selectedText);
    }

    if (isOpen) {
        target.draw(dropdownBackground);
        searchBox.draw(target);
        
        float pX = dropdownBackground.getPosition().x + 10;
        // Sửa 4 (tiếp): Dùng biến đã lưu
        float pY = searchBoxPos.y + searchBoxSize.y + 10;
        float rowHeight = 30;
        
        // Sửa 8: Lỗi thứ tự sf::Text
        sf::Text itemText(font, "", 16);

        for (size_t i = 0; i < filteredItems.size() && i < 5; ++i) {
            itemText.setString(sf::String::fromUtf8(filteredItems[i]->displayText.begin(), filteredItems[i]->displayText.end()));
            // Sửa 3 (tiếp): Lỗi API setPosition
            itemText.setPosition(sf::Vector2f(pX, pY + i * rowHeight));
            
            if (i == hoveredDropdownIndex) {
                itemText.setFillColor(Config::AccentCyan);
            } else {
                itemText.setFillColor(Config::TextNormal);
            }
            target.draw(itemText);
        }
    }
}