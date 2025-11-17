// GUI/Components/TabManager.cpp
#include "TabManager.h"

TabManager::TabManager(sf::Font& font) 
    : font(font), 
      position(0, 0), 
      activeTabIndex(0) 
{
    // Thiet lap duong gach chan mac dinh
    activeLine.setFillColor(Config::AccentCyan);
    activeLine.setSize(sf::Vector2f(0, 3)); // Chieu cao 3px, rong 0
}

TabManager::~TabManager() {}

void TabManager::setPosition(float x, float y) {
    position = sf::Vector2f(x, y);
    
    // Cap nhat lai vi tri cua tat ca cac nhan tab da co
    float currentX = x;
    for (size_t i = 0; i < tabLabels.size(); ++i) {
        tabLabels[i].setPosition(sf::Vector2f(currentX, y));
        
        // SFML 3.0 API Change: Su dung .size.x
        currentX += tabLabels[i].getGlobalBounds().size.x + 30; // 30px padding
    }
    
    // Cap nhat lai vi tri duong gach chan
    if (!tabLabels.empty()) {
        // SFML 3.0 API Change: Su dung .position.x, .size.x, ...
        sf::FloatRect bounds = tabLabels[activeTabIndex].getGlobalBounds();
        activeLine.setSize(sf::Vector2f(bounds.size.x, 3));
        activeLine.setPosition(sf::Vector2f(
            bounds.position.x, 
            bounds.position.y + bounds.size.y + 5 // 5px duoi nhan
        ));
    }
}

void TabManager::addTab(const std::string& label) {
    // SFML 3.0: Chuyen std::string sang sf::String
    sf::String sfLabel = sf::String::fromUtf8(label.begin(), label.end());
    sf::Text newTab(font, sfLabel, 18);
    
    tabLabels.push_back(newTab);
    
    // Tu dong cap nhat vi tri
    setPosition(position.x, position.y);
    
    // Kich hoat va to mau
    if (tabLabels.size() == 1) { // Neu la tab dau tien
        activeTabIndex = 0;
        tabLabels[0].setFillColor(Config::TextNormal); // Mau sang
    } else {
        tabLabels.back().setFillColor(Config::TextMuted); // Mau mo
    }
}

int TabManager::getActiveTab() const {
    return activeTabIndex;
}

void TabManager::handleEvent(sf::Event event, sf::Vector2i mousePos) {
    // *** SFML 3.0 API CHANGE ***
    if (auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            
            sf::Vector2f mousePosF(mousePos);
            
            for (size_t i = 0; i < tabLabels.size(); ++i) {
                if (tabLabels[i].getGlobalBounds().contains(mousePosF)) {
                    // Click vao tab nay
                    activeTabIndex = i;
                    
                    // Cap nhat giao dien
                    for (size_t j = 0; j < tabLabels.size(); ++j) {
                        tabLabels[j].setFillColor(Config::TextMuted); // Dat tat ca ve mau mo
                    }
                    tabLabels[i].setFillColor(Config::TextNormal); // Dat tab nay sang
                    
                    // Di chuyen duong gach chan
                    // *** SFML 3.0 API CHANGE ***
                    sf::FloatRect bounds = tabLabels[i].getGlobalBounds();
                    activeLine.setSize(sf::Vector2f(bounds.size.x, 3));
                    activeLine.setPosition(sf::Vector2f(
                        bounds.position.x, 
                        bounds.position.y + bounds.size.y + 5
                    ));
                    
                    return; // Da xu ly xong
                }
            }
        }
    }
}

void TabManager::draw(sf::RenderTarget& target) {
    // Ve tat ca cac nhan
    for (const auto& label : tabLabels) {
        target.draw(label);
    }
    // Ve duong gach chan
    target.draw(activeLine);
}